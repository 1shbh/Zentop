#pragma once

#include <string>

// Progress bar
std::string makeBar(double percent, int width = 20);

std::string formatPercent(double value);

// Trim string to fixed width
std::string formatColumn(const std::string &text, int width);

void clearScreen();

// Read terminal row count for dynamic layouts
int getTerminalRows();

// Color control
void setColorEnabled(bool enabled);
bool isColorEnabled();

std::string colorizeByLevel(const std::string &text,
                            double value,
                            double warn = 70.0,
                            double critical = 90.0);

// formatting helpers
std::string formatBytes(long long bytes);
std::string formatBytesPerSecond(double bytesPerSecond);