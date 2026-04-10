#include "memory_monitor.hpp"
#include "../core/system_parser.hpp"
#include "../utils/utils.hpp"

#include <iostream>

MemoryMonitor::MemoryMonitor() : usage(0.0) {}

void MemoryMonitor::update()
{
    long total = SystemParser::getTotalMemoryKB();
    long available = SystemParser::getAvailableMemoryKB();

    if (total > 0)
    {
        usage = (total - available) * 100.0 / total;
    }
}

void MemoryMonitor::render() const
{
    std::string percentText = formatPercent(usage);
    std::cout << "MEM    "
              << makeBar(usage)
              << " "
              << colorizeByLevel(percentText, usage)
              << "\n";
}