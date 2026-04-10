#pragma once

#include "../monitor.hpp"
#include <vector>

class CpuMonitor : public Monitor {
public:
    CpuMonitor();

    void update() override;
    void render() const override;

private:
    std::vector<long long> prevTimes;
    double usage;
};