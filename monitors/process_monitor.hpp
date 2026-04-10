#pragma once

#include "../monitor.hpp"
#include "../models/process.hpp"

#include <vector>
#include <unordered_map>

class ProcessMonitor : public Monitor
{
public:
    enum class SortBy
    {
        CPU,
        MEMORY
    };

    ProcessMonitor();

    void update() override;
    void render() const override;

    void setSortBy(SortBy mode);
    void toggleReverseSort();
    void setReverseSort(bool enable);
    void setMaxProcessesLimit(int limit);

    SortBy getSortBy() const;
    bool isReverseSort() const;

private:
    std::vector<Process> processes;

    std::unordered_map<int, long long> prevCpuTimes;
    long long prevTotalCpu = 0;

    int maxProcesses = 15;
    int maxProcessesLimit = 0;

    SortBy sortBy = SortBy::CPU;
    bool reverse = false;

    void computeCpuUsage(std::vector<Process> &procs, long long totalDelta);
    void computeMemoryUsage(std::vector<Process> &procs);
    void sortProcesses();
    void adjustProcessLimitToTerminal();

    std::string getHeader() const;
};