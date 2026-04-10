#include "utils.hpp"

#include <sstream>
#include <iomanip>
#include <iostream>
#include <array>
#include <sys/ioctl.h>
#include <unistd.h>

namespace
{
    bool g_colorEnabled = true;

    std::string colorWrap(const std::string &text, const char *code)
    {
        if (!g_colorEnabled)
        {
            return text;
        }
        return std::string(code) + text + "\033[0m";
    }
}

// █ = filled, ░ = empty
std::string makeBar(double percent, int width)
{
    if (percent < 0)
        percent = 0;
    if (percent > 100)
        percent = 100;

    int filled = static_cast<int>((percent / 100.0) * width);

    std::string bar = "[";
    for (int i = 0; i < width; i++)
    {
        if (i < filled)
            bar += "█";
        else
            bar += "░";
    }
    bar += "]";

    return bar;
}

std::string formatPercent(double value)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value << "%";
    return oss.str();
}

std::string formatColumn(const std::string &text, int width)
{
    if ((int)text.size() >= width)
    {
        return text.substr(0, width - 1) + " ";
    }

    return text + std::string(width - text.size(), ' ');
}

void clearScreen()
{
    std::cout << "\033[H\033[J" << std::flush;
}

int getTerminalRows()
{
    winsize ws{};
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_row > 0)
    {
        return ws.ws_row;
    }

    return 24;
}

void setColorEnabled(bool enabled)
{
    g_colorEnabled = enabled;
}

bool isColorEnabled()
{
    return g_colorEnabled;
}

std::string colorizeByLevel(const std::string &text, double value, double warn, double critical)
{
    if (value >= critical)
    {
        return colorWrap(text, "\033[31m");
    }
    if (value >= warn)
    {
        return colorWrap(text, "\033[33m");
    }

    return colorWrap(text, "\033[32m");
}

std::string formatBytes(long long bytes)
{
    static const std::array<const char *, 5> units = {"B", "KB", "MB", "GB", "TB"};
    double value = static_cast<double>(bytes);
    std::size_t unitIndex = 0;

    while (value >= 1024.0 && unitIndex + 1 < units.size())
    {
        value /= 1024.0;
        ++unitIndex;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(value >= 100.0 ? 0 : 1) << value << " " << units[unitIndex];
    return oss.str();
}

std::string formatBytesPerSecond(double bytesPerSecond)
{
    return formatBytes(static_cast<long long>(bytesPerSecond)) + "/s";
}