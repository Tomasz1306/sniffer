//
// Created by tomasz on 9/1/24.
//

#include "controllers/DeviceController.h"


DeviceController::DeviceController(std::shared_ptr<DeviceModel> _deviceModel, std::shared_ptr<DeviceView> _deviceView) {
    this->deviceModel = _deviceModel;
    this->deviceView = _deviceView;
}
