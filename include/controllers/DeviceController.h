//
// Created by tomasz on 9/1/24.
//

#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include "models/DeviceModel.h"
#include "views/DeviceView.h"
#include <memory>

class DeviceController {
public:

        DeviceController(std::shared_ptr<DeviceModel> _deviceModel, std::shared_ptr<DeviceView> _deviceView);

private:

        std::shared_ptr<DeviceModel> deviceModel;
        std::shared_ptr<DeviceView> deviceView;

};



#endif //DEVICECONTROLLER_H
