#pragma once

#include "../monitor.hpp"

class DiskMonitor : public Monitor
{
public:
    void update() override;
    void render() const override;

private:
    long long prevRead = 0, prevWrite = 0;
    long long totalRead = 0, totalWrite = 0;
    double readRate = 0, writeRate = 0;
};