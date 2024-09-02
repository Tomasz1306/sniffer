//
// Created by tomasz on 9/1/24.
//

#include "controllers/DeviceController.h"


DeviceController::DeviceController(std::shared_ptr<DeviceModel> _deviceModel, std::shared_ptr<DeviceView> _deviceView, std::shared_ptr<Listener> _listener) {
    this->deviceModel = _deviceModel;
    this->deviceView = _deviceView;
    this->listener = _listener;
}

void DeviceController::display() {
    this->deviceView->draw(this->getController(), this->deviceModel);
}
