#pragma once

#include <string>

struct Process {
    int pid = 0;
    std::string name;
    std::string user;
    char state = '?';

    double cpuPercent = 0.0;
    double memPercent = 0.0;

    long memoryKB = 0;

    long utime = 0;
    long stime = 0;

    // For sorting
    static bool compareByCPU(const Process& a, const Process& b) {
        return a.cpuPercent > b.cpuPercent;
    }

    static bool compareByMemory(const Process& a, const Process& b) {
        return a.memPercent > b.memPercent;
    }
};