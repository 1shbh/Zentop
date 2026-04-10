#pragma once

#include "../monitor.hpp"

class NetworkMonitor : public Monitor
{
public:
    void update() override;
    void render() const override;

private:
    long long prevRx = 0, prevTx = 0;
    long long totalRx = 0, totalTx = 0;
    double rxRate = 0, txRate = 0;
};