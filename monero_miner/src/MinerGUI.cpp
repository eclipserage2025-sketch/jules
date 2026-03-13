#include "MinerGUI.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>

MinerGUI::MinerGUI() : window(nullptr) {}

MinerGUI::~MinerGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    if (window) glfwDestroyWindow(window);
    glfwTerminate();
}

bool MinerGUI::initialize() {
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(800, 600, "AI Monero Miner Pro", NULL, NULL);
    if (!window) return false;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Apply Minimalist Dark Theme
    ImGui::StyleColorsDark();
    auto& style = ImGui::GetStyle();
    style.WindowRounding = 8.0f;
    style.FrameRounding = 4.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    return true;
}

void MinerGUI::render(double hashrate, int threads, const std::string& status, float predictedHashrate) {
    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    hashrateHistory.push_back((float)hashrate);
    if (hashrateHistory.size() > historyLimit) hashrateHistory.pop_front();

    // Main Dashboard Window
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Dashboard", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::TextColored(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), "AI MONERO MINER PRO v2.0");
    ImGui::Separator();
    ImGui::Spacing();

    // Metrics Row
    ImGui::Columns(3, "Metrics", false);
    ImGui::Text("Hashrate");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "%.2f H/s", hashrate);
    ImGui::NextColumn();
    ImGui::Text("Threads");
    ImGui::TextColored(ImVec4(1, 1, 1, 1), "%d", threads);
    ImGui::NextColumn();
    ImGui::Text("AI Prediction");
    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.4f, 1.0f), "%.2f H/s", predictedHashrate);
    ImGui::Columns(1);

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Spacing();

    // Performance Graph
    std::vector<float> graphData(hashrateHistory.begin(), hashrateHistory.end());
    ImGui::PlotLines("Performance (H/s)", graphData.data(), graphData.size(), 0, NULL, 0.0f, 1000.0f, ImVec2(0, 150));

    ImGui::Spacing();
    ImGui::Text("Status: %s", status.c_str());

    ImGui::End();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

bool MinerGUI::shouldClose() {
    return glfwWindowShouldClose(window);
}
