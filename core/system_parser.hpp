#pragma once

#include <vector>
#include <string>
#include "../models/process.hpp"

class SystemParser {
public:
    // CPU 
    static std::vector<long long> getCpuTimes();
    static long long getTotalCpuTime(const std::vector<long long>& times);
    static long long getIdleCpuTime(const std::vector<long long>& times);

    // MEMORY 
    static long getTotalMemoryKB();
    static long getAvailableMemoryKB();
    static long getSwapTotalKB();
    static long getSwapFreeKB();

    // SYSTEM 
    static long getUptime();
    static std::string getLoadAverage();

    // NETWORK
    static std::pair<long long, long long> getNetworkBytes(); 

    // DISK
    static std::pair<long long, long long> getDiskBytes(); 

    // PROCESSES 
    static std::vector<int> getPids();
    static Process getProcess(int pid);

private:
    static std::string readFile(const std::string& path);
};