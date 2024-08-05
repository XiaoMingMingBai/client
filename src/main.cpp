#include <iostream>
#include <thread>

#include "device/deviceInfo.hpp"
#include "transmit/api.hpp"

int main()
{
    DeviceInfo         device;
    WebServer          web("xiaomingmingbai", "love1999");
    nlohmann::json     deviceInfo;
    std::ostringstream usageStream;

    while (true)
    {
        usageStream << std::fixed << std::setprecision(2) << device.getCPUUsage();
        deviceInfo["cpu_usage"] = usageStream.str();
        usageStream.str("");

        usageStream << std::fixed << std::setprecision(2) << device.getGPUUsage();
        deviceInfo["gpu_usage"] = usageStream.str();
        usageStream.str("");

        std::cout << "json data: " << deviceInfo.dump(4) << std::endl;
        web.updataDeviceInfo(deviceInfo);

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    return 0;
}