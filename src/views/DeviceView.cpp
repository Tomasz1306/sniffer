//
// Created by tomasz on 9/1/24.
//

#include "views/DeviceView.h"

#include <iostream>

DeviceView::DeviceView() {

}

void DeviceView::draw(std::shared_ptr<DeviceController> _controller, std::shared_ptr<DeviceModel> _model) {
    //TODO przeniesc packeWindowInitialized z globals do tej instacji jako pole
    ImGui::SetWindowSize("DEVICES", ImVec2(1200.0f, 200.0f));
    if (!packetWindowInitialized) {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        packetWindowInitialized = true;
    }

    ImGui::Begin("DEVICES", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);

    const char* devices[this->deviceNames.size()];
    for (int i = 0; i < this->deviceNames.size(); i++) {
        devices[i] = this->deviceNames[i].c_str();
    }
    if (ImGui::Combo("Devices", &this->selectedDevice, devices, IM_ARRAYSIZE(devices))) {
        if (_controller->getCurrentDeviceName() != "") {
            if (this->deviceNames[this->selectedDevice] != _controller->getCurrentDeviceName()) {
                for (int i = 0; i < this->deviceNames.size(); i++) {
                    if (this->deviceNames[i] == _controller->getCurrentDeviceName()) {
                        _controller->setNewDeviceByName(this->deviceNames[i]);
                        _controller->openCurrentDevice();
                    }
                }
            }
        } else {
            _controller->setNewDeviceByName(this->deviceNames[this->selectedDevice]);
            _controller->openCurrentDevice();
        }
    }

    ImGui::End();
}

void DeviceView::fillDeviceNames(std::shared_ptr<DeviceModel> _model) {
    this->deviceNames.clear();
    for (int i = 0; i < _model->getDevices().size(); i++) {
        this->deviceNames.emplace(i, _model->getDevices()[i]->getName().c_str());
    }
}
