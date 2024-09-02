//
// Created by tomasz on 9/1/24.
//

#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include "controllers/DeviceController.h"
#include <PcapLiveDeviceList.h>
#include <PcapLiveDevice.h>
#include <vector>
#include <memory>

class DeviceController;

class DeviceModel {
public:

    DeviceModel();

    void setController(std::shared_ptr<DeviceController> _controller);
    void setDevice(pcpp::PcapLiveDevice* _device);
    void setDevices(std::vector<pcpp::PcapLiveDevice *> _devices);

private:

    std::shared_ptr<DeviceController> controller;
    pcpp::PcapLiveDevice *dev{nullptr};
    std::vector<pcpp::PcapLiveDevice *> devices;

};



#endif //DEVICEMODEL_H
