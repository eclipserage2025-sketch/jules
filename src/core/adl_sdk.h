#pragma once

#ifdef _WIN32
// Minimal ADL (AMD Display Library) stubs for compilation
typedef struct {
    int iAdapterIndex;
    int iTemperature;
} ADLTemperature;

typedef int (*ADL_MAIN_CONTROL_CREATE)(void*, int);
typedef int (*ADL_OVERDRIVE5_TEMPERATURE_GET)(int, int, ADLTemperature*);
#endif
