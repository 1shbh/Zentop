#include "disk_monitor.hpp"
#include "../core/system_parser.hpp"
#include "../utils/utils.hpp"

#include <iostream>

void DiskMonitor::update()
{
    auto [r, w] = SystemParser::getDiskBytes();
    totalRead = r;
    totalWrite = w;

    if (prevRead != 0)
    {
        readRate = static_cast<double>(r - prevRead);
        writeRate = static_cast<double>(w - prevWrite);
    }

    prevRead = r;
    prevWrite = w;
}

void DiskMonitor::render() const
{
    std::cout << "DISK    R: " << formatBytesPerSecond(readRate)
              << "   W: " << formatBytesPerSecond(writeRate)
              << "   total " << formatBytes(totalRead + totalWrite)
              << "\n";
}