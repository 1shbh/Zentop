#pragma once

#include "../monitor.hpp"

class MemoryMonitor : public Monitor {
public:
    MemoryMonitor();

    void update() override;
    void render() const override;

private:
    double usage;
};