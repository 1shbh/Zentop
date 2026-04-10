#include "process_monitor.hpp"
#include "../core/system_parser.hpp"
#include "../utils/utils.hpp"

#include <iostream>
#include <algorithm>

void ProcessMonitor::adjustProcessLimitToTerminal()
{
    constexpr int fallbackRows = 24;
    constexpr int nonProcessLines = 13;
    constexpr int minProcessRows = 3;
    constexpr int safetyLines = 1;

    int rows = getTerminalRows();
    if (rows <= 0)
    {
        rows = fallbackRows;
    }

    int availableRows = std::max(minProcessRows, rows - nonProcessLines - safetyLines);
    if (maxProcessesLimit > 0)
    {
        maxProcesses = std::min(availableRows, maxProcessesLimit);
    }
    else
    {
        maxProcesses = availableRows;
    }
}

void ProcessMonitor::setSortBy(SortBy mode)
{
    sortBy = mode;
}

void ProcessMonitor::toggleReverseSort()
{
    reverse = !reverse;
}

void ProcessMonitor::setReverseSort(bool enable)
{
    reverse = enable;
}

void ProcessMonitor::setMaxProcessesLimit(int limit)
{
    maxProcessesLimit = std::max(0, limit);
}

ProcessMonitor::SortBy ProcessMonitor::getSortBy() const
{
    return sortBy;
}

bool ProcessMonitor::isReverseSort() const
{
    return reverse;
}

ProcessMonitor::ProcessMonitor()
{
    prevTotalCpu = SystemParser::getTotalCpuTime(SystemParser::getCpuTimes());
}

void ProcessMonitor::update()
{
    auto pids = SystemParser::getPids();
    std::vector<Process> procs;

    for (int pid : pids)
    {
        Process p = SystemParser::getProcess(pid);
        if (p.pid > 0)
        {
            procs.push_back(p);
        }
    }

    long long currTotalCpu = SystemParser::getTotalCpuTime(SystemParser::getCpuTimes());
    long long totalDelta = currTotalCpu - prevTotalCpu;

    computeCpuUsage(procs, totalDelta);
    computeMemoryUsage(procs);

    processes = procs;

    sortProcesses();
    adjustProcessLimitToTerminal();

    if ((int)processes.size() > maxProcesses)
    {
        processes.resize(maxProcesses);
    }

    prevTotalCpu = currTotalCpu;
}

void ProcessMonitor::computeCpuUsage(std::vector<Process> &procs, long long totalDelta)
{
    for (auto &p : procs)
    {
        long long curr = p.utime + p.stime;

        long long prev = 0;
        if (prevCpuTimes.count(p.pid))
        {
            prev = prevCpuTimes[p.pid];
        }

        long long delta = curr - prev;

        if (totalDelta > 0)
        {
            p.cpuPercent = (double)delta * 100.0 / totalDelta;
        }

        prevCpuTimes[p.pid] = curr;
    }
}

void ProcessMonitor::computeMemoryUsage(std::vector<Process> &procs)
{
    long totalMem = SystemParser::getTotalMemoryKB();

    for (auto &p : procs)
    {
        if (totalMem > 0)
        {
            p.memPercent = (double)p.memoryKB * 100.0 / totalMem;
        }
    }
}

void ProcessMonitor::sortProcesses()
{
    if (sortBy == SortBy::CPU)
    {
        std::sort(processes.begin(), processes.end(), Process::compareByCPU);
    }
    else
    {
        std::sort(processes.begin(), processes.end(), Process::compareByMemory);
    }

    if (reverse)
    {
        std::reverse(processes.begin(), processes.end());
    }
}

std::string ProcessMonitor::getHeader() const
{
    return formatColumn("PID", 8) +
           formatColumn("USER", 12) +
           formatColumn("CPU%", 10) +
           formatColumn("MEM%", 10) +
           formatColumn("STATE", 8) +
           formatColumn("NAME", 20);
}

void ProcessMonitor::render() const
{
    std::cout << "=== PROCESSES (SORT: "
              << (sortBy == SortBy::CPU ? "CPU" : "MEM")
              << (reverse ? " ASC" : " DESC")
              << ") ===\n";
    std::cout << getHeader() << "\n";

    for (const auto &p : processes)
    {
        std::string cpuText = colorizeByLevel(formatColumn(formatPercent(p.cpuPercent), 10), p.cpuPercent);
        std::string memText = colorizeByLevel(formatColumn(formatPercent(p.memPercent), 10), p.memPercent);

        std::cout << formatColumn(std::to_string(p.pid), 8)
                  << formatColumn(p.user, 12)
                  << cpuText
                  << memText
                  << formatColumn(std::string(1, p.state), 8)
                  << formatColumn(p.name, 20)
                  << "\n";
    }
}