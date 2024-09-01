//
// Created by tomasz on 9/1/24.
//

#include <PcapLiveDevice.h>
#include <PcapLiveDeviceList.h>
#include <vector>

#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

class DeviceModel {
public:

    DeviceModel();

private:

    std::vector<pcpp::PcapLiveDevice *> devices;

};



#endif //DEVICEMODEL_H
