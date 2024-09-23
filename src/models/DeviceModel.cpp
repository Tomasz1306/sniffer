//
// Created by tomasz on 9/1/24.
//

#include "models/DeviceModel.h"

DeviceModel::DeviceModel() {

}

void DeviceModel::setController(std::shared_ptr<DeviceController> _controller) {
    this->controller = _controller;
}

void DeviceModel::setDevice(pcpp::PcapLiveDevice *_device) {
    this->dev = _device;
}

void DeviceModel::setDevices(std::vector<pcpp::PcapLiveDevice *> &_devices) {
    for (int i = 0; i < _devices.size(); i++) {
        this->devices.push_back(_devices[i]);
    }
}

std::vector<pcpp::PcapLiveDevice *> &DeviceModel::getDevices() {
    return this->devices;
}

pcpp::PcapLiveDevice *DeviceModel::getDevice() {
    if (this->dev != nullptr) {
        return this->dev;
    }
    return nullptr;
}

std::vector<uint64_t> DeviceModel::getDeviceStats() {
    if (this->dev == nullptr) {
        return std::vector<uint64_t>();
    }

    this->dev->getStatistics(this->stats);
    std::vector<uint64_t> statistics{this->stats.packetsRecv, this->stats.packetsDrop, this->stats.packetsDropByInterface};
    return statistics;
}

std::string DeviceModel::getDeviceIPv4Address() {
    if (this->dev == nullptr) {
        return "";
    }
    return this->dev->getIPv4Address().toString();
}

std::string DeviceModel::getDeviceIPv6Address() {
    if (this->dev == nullptr) {
        return "";
    }
    return this->dev->getIPv6Address().toString();
}

std::string DeviceModel::getDeviceMacAddress() {
    if (this->dev == nullptr) {
        return "";
    }
    return this->dev->getMacAddress().toString();
}

std::string DeviceModel::getDefaultGateway() {
    if (this->dev == nullptr) {
        return "";
    }
    return this->dev->getDefaultGateway().toString();
}

std::vector<std::string> DeviceModel::getDnsServers() {
    if (this->dev == nullptr) {
        return std::vector<std::string>();
    }
    std::vector<std::string> dnsServersString;
    std::vector<pcpp::IPv4Address> dnsServers;
    dnsServers = this->dev->getDnsServers();
    for (int i = 0; i < dnsServers.size(); i++) {
        dnsServersString.push_back(dnsServers[i].toString());
    }
    return dnsServersString;
}




