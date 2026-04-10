#include "cpu_monitor.hpp"
#include "../core/system_parser.hpp"
#include "../utils/utils.hpp"

#include <iostream>

CpuMonitor::CpuMonitor() : usage(0.0)
{
    prevTimes = SystemParser::getCpuTimes();
}

void CpuMonitor::update()
{
    auto currTimes = SystemParser::getCpuTimes();

    long long prevTotal = SystemParser::getTotalCpuTime(prevTimes);
    long long currTotal = SystemParser::getTotalCpuTime(currTimes);

    long long prevIdle = SystemParser::getIdleCpuTime(prevTimes);
    long long currIdle = SystemParser::getIdleCpuTime(currTimes);

    long long deltaTotal = currTotal - prevTotal;
    long long deltaIdle = currIdle - prevIdle;

    if (deltaTotal > 0)
    {
        usage = (deltaTotal - deltaIdle) * 100.0 / deltaTotal;
    }

    prevTimes = currTimes;
}

void CpuMonitor::render() const
{
    std::string percentText = formatPercent(usage);
    std::cout << "CPU    "
              << makeBar(usage)
              << " "
              << colorizeByLevel(percentText, usage)
              << "\n";
}