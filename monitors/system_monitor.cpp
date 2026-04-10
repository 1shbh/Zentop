#include "system_monitor.hpp"
#include "../core/system_parser.hpp"

#include <iostream>

void SystemMonitor::update() {
    uptime = SystemParser::getUptime();
    load = SystemParser::getLoadAverage();
}

void SystemMonitor::render() const {
    long hours = uptime / 3600;
    long minutes = (uptime % 3600) / 60;

    std::cout << "UPTIME  " << hours << "h " << minutes << "m\n";
    std::cout << "LOAD    " << load.substr(0, load.find('\n')) << "\n";
}