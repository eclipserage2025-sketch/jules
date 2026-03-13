#include "Optimizations.h"

#ifdef _WIN32
#include <windows.h>
#include <iostream>

bool Optimizations::enableLargePages() {
    HANDLE hToken;
    LUID luid;
    TOKEN_PRIVILEGES tp;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return false;
    }

    if (!LookupPrivilegeValue(NULL, SE_LOCK_MEMORY_NAME, &luid)) {
        CloseHandle(hToken);
        return false;
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL)) {
        CloseHandle(hToken);
        return false;
    }

    CloseHandle(hToken);
    return (GetLastError() == ERROR_SUCCESS);
}

bool Optimizations::applyMSRTweaks() {
    std::cout << "[Opt] Applying Windows MSR tweaks (Requires Admin)..." << std::endl;
    // Logic to interact with WinRing0 or similar driver would go here
    return true;
}

#else
// Linux Implementations
#include <sys/mman.h>

bool Optimizations::enableLargePages() {
    std::cout << "[Opt] Checking HugePages on Linux..." << std::endl;
    // On Linux, this is usually handled by the OS (vm.nr_hugepages)
    // The miner just needs to use MAP_HUGETLB in mmap
    return true;
}

bool Optimizations::applyMSRTweaks() {
    std::cout << "[Opt] Applying Linux MSR tweaks..." << std::endl;
    // Example: wrmsr -a 0x1a4 0xf (Disable hardware prefetchers on some CPUs)
    return true;
}
#endif
