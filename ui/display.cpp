#include "display.hpp"
#include "../utils/utils.hpp"

#include <iostream>

void Display::render(const CpuMonitor &cpu,
                     const MemoryMonitor &mem,
                     const ProcessMonitor &proc,
                     const SystemMonitor &sys,
                     const NetworkMonitor &net,
                     const DiskMonitor &disk) const
{

    clearScreen();

    std::cout << "========== SYSTEM MONITOR ==========\n\n";

    cpu.render();
    mem.render();

    std::cout << "\n";
    sys.render();

    std::cout << "\n";
    net.render();
    disk.render();

    std::cout << "[Keys] q quit | c CPU sort | m MEM sort | r reverse | + faster | - slower\n";

    proc.render();
    std::cout << std::flush;
}