//
// Created by tomasz on 9/1/24.
//

#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <PcapLiveDevice.h>

#include <vector>
#include <memory>

class DeviceController;

class DeviceModel {
public:

    DeviceModel();
    void setController(std::shared_ptr<DeviceController> _controller);
    void setDevice(pcpp::PcapLiveDevice* _device);
    void setDevices(std::vector<pcpp::PcapLiveDevice *> &_devices);
    std::vector<pcpp::PcapLiveDevice *> &getDevices();
    pcpp::PcapLiveDevice *getDevice();
    std::vector<uint64_t> getDeviceStats();
    std::string getDeviceIPv4Address();
    std::string getDeviceIPv6Address();
    std::string getDefaultGateway();
    std::string getDeviceMacAddress();
    std::vector<std::string> getDnsServers();
    int getMtu();


private:
    pcpp::IPcapDevice::PcapStats stats;
    std::shared_ptr<DeviceController> controller;
    pcpp::PcapLiveDevice *dev{nullptr};
    std::vector<pcpp::PcapLiveDevice *> devices;
};

#endif //DEVICEMODEL_H
