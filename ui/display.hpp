#pragma once

#include "../monitors/cpu_monitor.hpp"
#include "../monitors/memory_monitor.hpp"
#include "../monitors/process_monitor.hpp"
#include "../monitors/system_monitor.hpp"
#include "../monitors/network_monitor.hpp"
#include "../monitors/disk_monitor.hpp"

class Display {
public:
    void render(const CpuMonitor& cpu,
                const MemoryMonitor& mem,
                const ProcessMonitor& proc,
                const SystemMonitor& sys,
                const NetworkMonitor& net,
                const DiskMonitor& disk) const;
};