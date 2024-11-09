//
// Created by tomasz on 9/1/24.
//

#ifndef DEVICEVIEW_H
#define DEVICEVIEW_H

#include "controllers/DeviceController.h"
#include "models/DeviceModel.h"
#include "views/View.h"

#include <memory>
#include <string>
#include <map>

class DeviceController;
class DeviceModel;

class DeviceView : public View {
public:

    DeviceView();
    void fillDeviceNames(std::shared_ptr<DeviceModel> _model);
    void draw(std::shared_ptr<DeviceController> _controller, std::shared_ptr<DeviceModel> _model);

private:
    bool packetWindowInitialized{false};
    int selectedDevice{-1};
    std::map<int, std::string> deviceNames;
};

#endif //DEVICEVIEW_H
