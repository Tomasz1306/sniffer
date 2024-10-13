//
// Created by tomasz on 9/1/24.
//

#include "controllers/DeviceController.h"


DeviceController::DeviceController(std::shared_ptr<DeviceModel> _deviceModel, std::shared_ptr<DeviceView> _deviceView, std::shared_ptr<Listener> _listener) {
    this->deviceModel = _deviceModel;
    this->deviceView = _deviceView;
    this->listener = _listener;
    this->deviceModel->setDevices(this->listener->getDevices());
    this->deviceView->fillDeviceNames(this->deviceModel);
}

std::vector<pcpp::PcapLiveDevice*> & DeviceController::getDevices() {
    return this->deviceModel->getDevices();
}

void DeviceController::display() {
    this->deviceView->draw(this->getController(), this->deviceModel);
}

void DeviceController::setNewDeviceByName(std::string _devName) {
    this->listener->setDeviceByName(_devName);
    this->deviceModel->setDevice(this->listener->getDevice());
}

std::string DeviceController::getCurrentDeviceName() {
    if (this->deviceModel->getDevice() == nullptr) {
        return "";
    }
    if (this->deviceModel->getDevice() != nullptr) {
        return this->deviceModel->getDevice()->getName();
    }
    return "";
}

void DeviceController::openCurrentDevice() {
    this->listener->openListener();
}

void DeviceController::closeCurrentDevice() {
    this->listener->closeListener();
}

bool DeviceController::isDeviceOpen() {
    return this->m_isDeviceOpen;
}

void DeviceController::setIsDeviceOpen(bool _isDeviceOpen) {
    this->m_isDeviceOpen = _isDeviceOpen;
}




