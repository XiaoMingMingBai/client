#if !defined(__DEVICEINFO_CPP__)
#define __DEVICEINFO_CPP__

#include <cmath>
#include <string>

class DataSpace
{
  private:

  public:
    int dataKb;


    int Kb(void)
    {
        return dataKb;
    }
    float Mb(const short precision = 2)
    {
        return std::round(dataKb / 1024.0 * std::pow(10, precision)) / std::pow(10, precision);
    }
    float Gb(const short precision = 4)
    {
        return std::round(dataKb / 1024.0 / 1024.0 * std::pow(10, precision)) / std::pow(10, precision);
    }


    DataSpace(/* args */);
    ~DataSpace();
    // 声明赋值操作符重载
    DataSpace& operator=(const int& other);
};

class DeviceBase
{
  private:
    void getOSInfo(void);
    void getCPUInfo(void);
    void getGPUInfo(void);
    void getMemoryInfo(void);
    void getDiskInfo(void);

  public:
    std::string hostName;

    std::string osPretiyName;
    std::string osName;
    std::string osVersion;
    std::string osID;
    std::string osIDLike;
    std::string osHomeURL;

    std::string cpuName;
    short       cpuCores;
    short       cpuThreads;
    std::string cpuFreq;
    float       cpuUsage;

    std::string gpuName;
    DataSpace   gpuMemory;
    float       gpuUsage;

    DataSpace   memoryTotal;
    DataSpace   memoryAvailable;
    float memoryUsage;

    DataSpace   diskTotal;
    DataSpace   diskAvailable;
    float diskUsage;

    float getCPUUsage(void);
    float getGPUUsage(void);
    float getMemoryUsage(void);
    float getDiskUsage(void);

    short funcOnoff(const bool onoff);
    short funcReboot(void);

    DeviceBase(/* args */);
    ~DeviceBase();
};

class DeviceInfo : public DeviceBase
{
  private:
    /* data */
  public:
    DeviceInfo(/* args */);
    ~DeviceInfo();
};

#endif // __DEVICEINFO_CPP__
