//
// Created by tomasz on 9/1/24.
//

#ifndef DEVICEVIEW_H
#define DEVICEVIEW_H

#include <controllers/DeviceController.h>
#include "models/DeviceModel.h"


#include "imgui.h"
#include <memory>

class DeviceController;
class DeviceModel;

class DeviceView {
public:

    DeviceView();

    void draw(std::shared_ptr<DeviceController> _controller, std::shared_ptr<DeviceModel> _model);

private:
    bool packetWindowInitialized{false};
};

#endif //DEVICEVIEW_H
