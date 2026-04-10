#include "network_monitor.hpp"
#include "../core/system_parser.hpp"
#include "../utils/utils.hpp"

#include <iostream>

void NetworkMonitor::update()
{
    auto [rx, tx] = SystemParser::getNetworkBytes();
    totalRx = rx;
    totalTx = tx;

    if (prevRx != 0)
    {
        rxRate = static_cast<double>(rx - prevRx);
        txRate = static_cast<double>(tx - prevTx);
    }

    prevRx = rx;
    prevTx = tx;
}

void NetworkMonitor::render() const
{
    std::cout << "NET     ↓ " << formatBytesPerSecond(rxRate)
              << "   ↑ " << formatBytesPerSecond(txRate)
              << "   total " << formatBytes(totalRx + totalTx)
              << "\n";
}