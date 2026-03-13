#ifndef OPTIMIZATIONS_H
#define OPTIMIZATIONS_H

#include <iostream>

class Optimizations {
public:
    static bool enableLargePages();
    static bool applyMSRTweaks();
};

#endif
