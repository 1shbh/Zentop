#include "monitors/cpu_monitor.hpp"
#include "monitors/memory_monitor.hpp"
#include "monitors/process_monitor.hpp"
#include "monitors/system_monitor.hpp"
#include "monitors/network_monitor.hpp"
#include "monitors/disk_monitor.hpp"
#include "ui/display.hpp"
#include "utils/utils.hpp"

#include <thread>
#include <chrono>
#include <string>
#include <algorithm>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

namespace
{
    struct TerminalState
    {
        termios oldt{};
        int oldFlags = 0;
        bool configured = false;
    };

    TerminalState g_terminalState;
}

void printUsage(const char *progName)
{
    std::cout << "Usage: " << progName << " [options]\n"
              << "  --interval <sec>   Refresh interval in seconds (default: 1)\n"
              << "  --top <n>          Max process rows to show (default: auto)\n"
              << "  --sort <cpu|mem>   Initial process sort (default: cpu)\n"
              << "  --reverse          Reverse sort order\n"
              << "  --no-color         Disable ANSI colors\n"
              << "  --help             Show this help\n";
}

bool parsePositiveInt(const std::string &text, int &out)
{
    try
    {
        int value = std::stoi(text);
        if (value <= 0)
        {
            return false;
        }
        out = value;
        return true;
    }
    catch (...)
    {
        return false;
    }
}

bool setNonBlocking(bool enable)
{
    if (enable)
    {
        if (!isatty(STDIN_FILENO))
        {
            return false;
        }

        struct termios newt;
        if (tcgetattr(STDIN_FILENO, &g_terminalState.oldt) == -1)
        {
            return false;
        }

        newt = g_terminalState.oldt;
        newt.c_lflag &= ~(ICANON | ECHO);

        if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) == -1)
        {
            return false;
        }

        g_terminalState.oldFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
        if (g_terminalState.oldFlags == -1)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &g_terminalState.oldt);
            return false;
        }

        if (fcntl(STDIN_FILENO, F_SETFL, g_terminalState.oldFlags | O_NONBLOCK) == -1)
        {
            tcsetattr(STDIN_FILENO, TCSANOW, &g_terminalState.oldt);
            return false;
        }

        g_terminalState.configured = true;
        return true;
    }

    if (g_terminalState.configured)
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &g_terminalState.oldt);
        fcntl(STDIN_FILENO, F_SETFL, g_terminalState.oldFlags);
        g_terminalState.configured = false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    int intervalMs = 1000;
    int topLimit = 0;
    ProcessMonitor::SortBy initialSort = ProcessMonitor::SortBy::CPU;
    bool reverseSort = false;
    bool colorEnabled = true;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "--help")
        {
            printUsage(argv[0]);
            return 0;
        }
        else if (arg == "--interval" && i + 1 < argc)
        {
            int sec = 0;
            if (!parsePositiveInt(argv[++i], sec))
            {
                std::cerr << "Invalid --interval value\n";
                return 1;
            }
            intervalMs = sec * 1000;
        }
        else if (arg == "--top" && i + 1 < argc)
        {
            if (!parsePositiveInt(argv[++i], topLimit))
            {
                std::cerr << "Invalid --top value\n";
                return 1;
            }
        }
        else if (arg == "--sort" && i + 1 < argc)
        {
            std::string mode = argv[++i];
            if (mode == "cpu")
            {
                initialSort = ProcessMonitor::SortBy::CPU;
            }
            else if (mode == "mem")
            {
                initialSort = ProcessMonitor::SortBy::MEMORY;
            }
            else
            {
                std::cerr << "Invalid --sort value. Use cpu or mem\n";
                return 1;
            }
        }
        else if (arg == "--reverse")
        {
            reverseSort = true;
        }
        else if (arg == "--no-color")
        {
            colorEnabled = false;
        }
        else
        {
            std::cerr << "Unknown option: " << arg << "\n";
            printUsage(argv[0]);
            return 1;
        }
    }

    CpuMonitor cpu;
    MemoryMonitor mem;
    ProcessMonitor proc;
    SystemMonitor sys;
    NetworkMonitor net;
    DiskMonitor disk;
    Display display;

    proc.setSortBy(initialSort);
    proc.setReverseSort(reverseSort);
    proc.setMaxProcessesLimit(topLimit);
    setColorEnabled(colorEnabled && isatty(STDOUT_FILENO));

    bool nonBlockingEnabled = setNonBlocking(true);
    std::cout << "\033[2J\033[H\033[?25l" << std::flush;

    bool running = true;

    while (running)
    {
        cpu.update();
        mem.update();
        proc.update();
        sys.update();
        net.update();
        disk.update();

        display.render(cpu, mem, proc, sys, net, disk);

        char ch = getchar();
        if (ch == 'q')
            running = false;
        else if (ch == 'c')
            proc.setSortBy(ProcessMonitor::SortBy::CPU);
        else if (ch == 'm')
            proc.setSortBy(ProcessMonitor::SortBy::MEMORY);
        else if (ch == 'r')
            proc.toggleReverseSort();
        else if (ch == '+')
            intervalMs = std::max(200, intervalMs - 200);
        else if (ch == '-')
            intervalMs = std::min(5000, intervalMs + 200);

        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
    }

    std::cout << "\033[?25h\n"
              << std::flush;
    if (nonBlockingEnabled)
    {
        setNonBlocking(false);
    }
    return 0;
}