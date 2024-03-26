#include "cpu.h"
#define KB 1024
#define MB (KB*1024)
#define GB (MB*1024)
#define TB (GB *1024)

#pragma comment(lib, "pdh.lib")

static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

void init() {
    PDH_STATUS status;

    status = PdhOpenQuery(NULL, NULL, &cpuQuery);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to open query. Error code: " << status << std::endl;
        return;
    }

    status = PdhAddEnglishCounter(cpuQuery, L"\\Processor(_Total)\\% Processor Time", NULL, &cpuTotal);
    if (status != ERROR_SUCCESS) {


        std::cerr << "Failed to add counter. Error code: " << status << std::endl;
        PdhCloseQuery(cpuQuery);
        return;
    }

    status = PdhCollectQueryData(cpuQuery);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to collect query data. Error code: " << status << std::endl;
        PdhCloseQuery(cpuQuery);
        return;
    }
}

double getCurrentValue() {
    PDH_FMT_COUNTERVALUE counterVal;

    PDH_STATUS status = PdhCollectQueryData(cpuQuery);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to collect query data. Error code: " << status << std::endl;
        return -1.0; // Return an error value
    }

    status = PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    if (status != ERROR_SUCCESS) {
        std::cerr << "Failed to get formatted counter value. Error code: " << status << std::endl;
        return -1.0; // Return an error value
    }

    return counterVal.doubleValue;
}


DWORDLONG getVirtualMemoryUsed() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;

    std::cout << "Current virtual memory used: " << virtualMemUsed / GB<< " GB" << std::endl;
    return virtualMemUsed;
}


DWORDLONG getPhysicalMemoryUseg() {


    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;


    std::cout << "Current physical memory used: " << physMemUsed / GB <<" GB" << std::endl;

    return physMemUsed;
}
