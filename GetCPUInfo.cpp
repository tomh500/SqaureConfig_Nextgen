#include <iostream>
#include <intrin.h> 

std::string getCPUVendor() {
    int cpuInfo[4];
    __cpuid(cpuInfo, 0);
    char vendor[13];
    memcpy(vendor, &cpuInfo[1], 4);
    memcpy(vendor + 4, &cpuInfo[3], 4);
    memcpy(vendor + 8, &cpuInfo[2], 4);
    vendor[12] = '\0'; 
    return std::string(vendor);
}
