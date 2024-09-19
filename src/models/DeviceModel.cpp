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