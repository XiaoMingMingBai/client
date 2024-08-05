#include "deviceInfo.hpp"
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/statvfs.h> // 包含 statvfs 函数的头文件
#include <thread>
#include <unistd.h> // 包含 gethostname 函数的头文件

/*============================================================================*/
DataSpace::DataSpace(/* args */) {}

DataSpace::~DataSpace() {}

// 定义赋值操作符重载
DataSpace& DataSpace::operator=(const int& other)
{
    this->dataKb = other;

    return *this;
}

/*============================================================================*/

void DeviceBase::getOSInfo(void)
{
    // 读取 /etc/os-release 文件
    std::ifstream file("/etc/os-release");
    std::string   line;
    std::getline(file, line);
    short count = 0;

    while (std::getline(file, line) && count < 6)
    {
        if (line.find("PRETTY_NAME=") == 0)
        {
            osPretiyName = line.substr(13, line.size() - 14);
            std::cout << "OS Pretty Name: " << osPretiyName << std::endl;
        }
        else if (line.find("NAME=") == 0)
        {
            osName = line.substr(6, line.size() - 7);
            std::cout << "OS Name: " << osName << std::endl;
        }
        else if (line.find("VERSION=") == 0)
        {
            osVersion = line.substr(9, line.size() - 10);
            std::cout << "OS Version: " << osVersion << std::endl;
        }
        else if (line.find("ID=") == 0)
        {
            osID = line.substr(3, line.size() - 4);
            std::cout << "OS ID: " << osID << std::endl;
        }
        else if (line.find("ID_LIKE=") == 0)
        {
            osIDLike = line.substr(8, line.size() - 9);
            std::cout << "OS ID Like: " << osIDLike << std::endl;
        }
        else if (line.find("HOME_URL=") == 0)
        {
            osHomeURL = line.substr(9, line.size() - 10);
            std::cout << "OS Home URL: " << osHomeURL << std::endl;
        }
        else
        {
            continue;
        }
        count++;
    }

    file.close();
}

void DeviceBase::getCPUInfo(void)
{
    // 读取 /proc/cpuinfo 文件
    std::ifstream file("/proc/cpuinfo");
    std::string   line;
    short         count = 0;

    while (std::getline(file, line) && count < 4)
    {
        if (line.find("model name") == 0)
        {
            cpuName = line.substr(13, line.size() - 13);
            std::cout << "CPU Name: " << cpuName << std::endl;
        }
        else if (line.find("cpu cores") == 0)
        {
            std::sscanf(line.c_str(), "cpu cores : %hd", &cpuCores);
            std::cout << "CPU Cores: " << cpuCores << std::endl;
        }
        else if (line.find("siblings") == 0)
        {
            std::sscanf(line.c_str(), "siblings : %hd", &cpuThreads);
            std::cout << "CPU Threads: " << cpuThreads << std::endl;
        }
        else if (line.find("cpu MHz") == 0)
        {
            cpuFreq = line.substr(11, line.size() - 11);
            std::cout << "CPU Frequency: " << cpuFreq << std::endl;
        }
        else
        {
            continue;
        }
        count++;
    }
    file.close();
}

void DeviceBase::getGPUInfo(void)
{
    // 读取 /proc/meminfo 文件
    std::ifstream file("/proc/meminfo");
    std::string   line;

    while (std::getline(file, line))
    {
        if (line.find("MemTotal:") == 0)
        {
            std::sscanf(line.c_str(), "MemTotal: %d kB", &(gpuMemory.dataKb));
            std::cout << "GPU Memory(Gb): " << gpuMemory.Gb(2) << " GB" << std::endl;
            break;
        }
    }
    file.close();
}

void DeviceBase::getMemoryInfo(void)
{
    // 读取 /proc/meminfo 文件
    std::ifstream file("/proc/meminfo");
    std::string   line;
    short         count = 0;

    while (std::getline(file, line) && count < 2)
    {
        if (line.find("MemTotal:") == 0)
        {
            std::sscanf(line.c_str(), "MemTotal: %d kB", &(memoryTotal.dataKb));
            std::cout << "Memory Total: " << memoryTotal.Gb(2) << " GB" << std::endl;
        }
        else if (line.find("MemAvailable:") == 0)
        {
            std::sscanf(line.c_str(), "MemAvailable: %d kB", &(memoryAvailable.dataKb));
            std::cout << "Memory Available: " << memoryAvailable.Gb(2) << " GB" << std::endl;
        }
        else
        {
            continue;
        }
        count++;
    }

    file.close();
}

void DeviceBase::getDiskInfo(void)
{
    struct statvfs stat;

    // 获取文件系统信息
    if (statvfs(std::string("/").c_str(), &stat) != 0)
    {
        // 处理错误
        std::cerr << "Failed to get disk space information for " << std::endl;
        return;
    }

    // 计算总空间和可用空间
    unsigned long long totalSpace     = stat.f_blocks * stat.f_frsize;
    unsigned long long availableSpace = stat.f_bavail * stat.f_frsize;

    // 将结果转换为 KB
    diskTotal     = totalSpace / 1024;
    diskAvailable = availableSpace / 1024;

    // 打印结果
    std::cout << "Disk Total: " << diskTotal.Gb(2) << " GB" << std::endl;
    std::cout << "Disk Available: " << diskAvailable.Gb(2) << " GB" << std::endl;
}

float DeviceBase::getCPUUsage(void)
{
    std::ifstream file("/proc/stat");
    std::string   line;
    std::getline(file, line);
    file.close();

    size_t user, nice, system, idle, iowait, irq, softirq, steal;
    // 提取 CPU 使用值
    std::sscanf(line.c_str(), "cpu  %zu %zu %zu %zu %zu %zu %zu %zu", &user, &nice, &system, &idle, &iowait, &irq, &softirq, &steal);
    size_t totalIdle    = idle + iowait;                                // 空闲时间
    size_t totalNonIdle = user + nice + system + irq + softirq + steal; // 非空闲时间
    size_t total        = totalIdle + totalNonIdle;                     // 总时间

    static size_t prevTotal = 0; // 上一次的总时间
    static size_t prevIdle  = 0; // 上一次的空闲时间

    float totalDiff = total - prevTotal;    // 总时间差
    float idleDiff  = totalIdle - prevIdle; // 空闲时间差

    prevTotal = total;     // 更新上一次的总时间
    prevIdle  = totalIdle; // 更新上一次的空闲时间

    cpuUsage = (float)(totalDiff - idleDiff) / totalDiff * 100.0; // 计算 CPU 使用率百分比

    return std::round(cpuUsage * std::pow(10, 2)) / std::pow(10, 2); // 计算并返回 CPU 使用率百分比
}

float DeviceBase::getGPUUsage(void)
{
    return 0.0;
}

float DeviceBase::getMemoryUsage(void)
{
    std::ifstream file("/proc/meminfo"); // 打开文件
    std::string   line;
    size_t        memTotal = 0, memFree = 0, buffers = 0, cached = 0;

    while (std::getline(file, line))
    {
        if (line.find("MemTotal:") == 0)
        {
            std::sscanf(line.c_str(), "MemTotal: %zu kB", &memTotal);
        }
        else if (line.find("MemFree:") == 0)
        {
            std::sscanf(line.c_str(), "MemFree: %zu kB", &memFree);
        }
        else if (line.find("Buffers:") == 0)
        {
            std::sscanf(line.c_str(), "Buffers: %zu kB", &buffers);
        }
        else if (line.find("Cached:") == 0)
        {
            std::sscanf(line.c_str(), "Cached: %zu kB", &cached);
        }
    }
    file.close();

    size_t usedMemory = memTotal - memFree - buffers - cached;

    memoryUsage = (float)usedMemory / memTotal * 100.0;

    return std::round(memoryUsage * std::pow(10, 2)) / std::pow(10, 2);
}

float DeviceBase::getDiskUsage(void)
{
    return 0.0f;
}

short DeviceBase::funcOnoff(const bool onoff)
{
    if (onoff)
    {
        std::cout << "System runing..." << std::endl;
        return system("sudo reboot");
    }
    else
    {
        std::cout << "Shutting down system..." << std::endl;
        return system("sudo shutdown -h now");
    }
}

short DeviceBase::funcReboot(void)
{
    std::cout << "Rebooting system..." << std::endl;
    return system("sudo reboot");
}

DeviceBase::DeviceBase(/* args */)
{
    // 获取hostname
    char hostname[128];
    hostname[127] = '\0';
    gethostname(hostname, 127);
    hostName = hostname;
    std::cout << "Hostname: " << hostName << std::endl;

    // 获取 OS 信息
    getOSInfo();

    // 获取 CPU 信息
    getCPUInfo();

    // 获取 GPU 信息
    getGPUInfo();

    // 获取内存信息
    getMemoryInfo();

    // 获取磁盘信息
    getDiskInfo();

    std::cout << "DeviceBase Constructor" << std::endl;
}

DeviceBase::~DeviceBase() {}

/*============================================================================*/
DeviceInfo::DeviceInfo(/* args */) : DeviceBase() {}

DeviceInfo::~DeviceInfo() {}
