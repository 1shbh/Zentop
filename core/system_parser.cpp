#include "system_parser.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <algorithm>
#include <cctype>

// Helper
std::string SystemParser::readFile(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return "";

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// CPU
std::vector<long long> SystemParser::getCpuTimes()
{
    std::ifstream file("/proc/stat");

    std::string line;
    std::getline(file, line);

    std::istringstream ss(line);
    std::string cpu;
    ss >> cpu;

    std::vector<long long> times;
    long long val;

    while (ss >> val)
        times.push_back(val);

    return times;
}

long long SystemParser::getTotalCpuTime(const std::vector<long long> &times)
{
    long long total = 0;
    for (auto t : times)
        total += t;
    return total;
}

long long SystemParser::getIdleCpuTime(const std::vector<long long> &times)
{
    if (times.size() < 5)
        return 0;
    return times[3] + times[4];
}

// MEMORY
long SystemParser::getTotalMemoryKB()
{
    std::ifstream file("/proc/meminfo");
    std::string key;
    long value;
    std::string unit;

    while (file >> key >> value >> unit)
    {
        if (key == "MemTotal:")
            return value;
    }
    return 0;
}

long SystemParser::getAvailableMemoryKB()
{
    std::ifstream file("/proc/meminfo");
    std::string key;
    long value;
    std::string unit;

    while (file >> key >> value >> unit)
    {
        if (key == "MemAvailable:")
            return value;
    }
    return 0;
}

long SystemParser::getSwapTotalKB()
{
    std::ifstream file("/proc/meminfo");
    std::string key;
    long value;
    std::string unit;

    while (file >> key >> value >> unit)
    {
        if (key == "SwapTotal:")
            return value;
    }
    return 0;
}

long SystemParser::getSwapFreeKB()
{
    std::ifstream file("/proc/meminfo");
    std::string key;
    long value;
    std::string unit;

    while (file >> key >> value >> unit)
    {
        if (key == "SwapFree:")
            return value;
    }
    return 0;
}

// SYSTEM
long SystemParser::getUptime()
{
    std::ifstream file("/proc/uptime");
    long uptime;
    file >> uptime;
    return uptime;
}

std::string SystemParser::getLoadAverage()
{
    return readFile("/proc/loadavg");
}

// NETWORK
std::pair<long long, long long> SystemParser::getNetworkBytes()
{
    std::ifstream file("/proc/net/dev");

    std::string line;
    long long rx = 0, tx = 0;

    std::getline(file, line);
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::istringstream ss(line);
        std::string iface;

        ss >> iface;

        if (iface.find("lo:") != std::string::npos)
            continue;

        long long r, t;
        ss >> r;
        for (int i = 0; i < 7; i++)
            ss >> line;
        ss >> t;

        rx += r;
        tx += t;
    }

    return {rx, tx};
}

// DISK
std::pair<long long, long long> SystemParser::getDiskBytes()
{
    std::ifstream file("/proc/diskstats");

    std::string line;
    long long read = 0, write = 0;

    while (std::getline(file, line))
    {
        std::istringstream ss(line);

        int major, minor;
        std::string name;

        ss >> major >> minor >> name;

        if (name.find("loop") != std::string::npos)
            continue;

        long long reads = 0, readMerged = 0, sectorsRead = 0, msReading = 0;
        long long writes = 0, writeMerged = 0, sectorsWritten = 0;

        if (!(ss >> reads >> readMerged >> sectorsRead >> msReading >> writes >> writeMerged >> sectorsWritten))
        {
            continue;
        }

        read += sectorsRead * 512;
        write += sectorsWritten * 512;
    }

    return {read, write};
}

// PROCESSES
std::vector<int> SystemParser::getPids()
{
    std::vector<int> pids;

    DIR *dir = opendir("/proc");
    if (!dir)
        return pids;

    struct dirent *entry;

    while ((entry = readdir(dir)) != nullptr)
    {
        if (entry->d_type == DT_DIR)
        {
            std::string name = entry->d_name;

            if (std::all_of(name.begin(), name.end(), [](unsigned char c)
                            { return std::isdigit(c) != 0; }))
            {
                pids.push_back(std::stoi(name));
            }
        }
    }

    closedir(dir);
    return pids;
}

Process SystemParser::getProcess(int pid)
{
    Process proc;
    proc.pid = pid;

    std::string stat = readFile("/proc/" + std::to_string(pid) + "/stat");
    std::size_t lparen = stat.find('(');
    std::size_t rparen = stat.rfind(')');

    if (lparen == std::string::npos || rparen == std::string::npos || rparen <= lparen)
    {
        return proc;
    }

    proc.name = stat.substr(lparen, rparen - lparen + 1);

    if (rparen + 2 < stat.size())
    {
        proc.state = stat[rparen + 2];
    }

    if (rparen + 4 < stat.size())
    {
        std::istringstream ss(stat.substr(rparen + 4));
        std::string temp;
        for (int i = 0; i < 10; i++)
        {
            if (!(ss >> temp))
            {
                return proc;
            }
        }
        ss >> proc.utime;
        ss >> proc.stime;
    }

    std::ifstream file("/proc/" + std::to_string(pid) + "/status");

    std::string key;
    while (file >> key)
    {
        if (key == "VmRSS:")
        {
            file >> proc.memoryKB;
        }
        else if (key == "Uid:")
        {
            int uid;
            file >> uid;

            struct passwd *pw = getpwuid(uid);
            if (pw)
                proc.user = pw->pw_name;
        }
    }

    return proc;
}