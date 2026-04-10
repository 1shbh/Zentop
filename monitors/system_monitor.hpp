#pragma once
#include <string>
#include "../monitor.hpp"

class SystemMonitor : public Monitor {
public:
    void update() override;
    void render() const override;

private:
    long uptime = 0;
    std::string load;
};