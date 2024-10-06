//
// Created by tomasz on 9/1/24.
//

#include "views/DeviceView.h"

#include <iostream>

#include "controllers/LogController.h"
#include "utils/Utils.h"

DeviceView::DeviceView() {
    this->windowTitle = "DEVICES";
    // this->windowHeight = 600.0f;
    // this->windowWidth = 200.0f;
    // this->windowX = 0.0f;
    // this->windowY = 0.0f;
    this->isWindowOpened = true;
    // this->windowFlags = ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar;
}

void DeviceView::draw(std::shared_ptr<DeviceController> _controller, std::shared_ptr<DeviceModel> _model) {
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);

    const char* devices[this->deviceNames.size()];
    for (int i = 0; i < this->deviceNames.size(); i++) {
        devices[i] = this->deviceNames[i].c_str();
    }
    int previousDeviceIndex = this->selectedDevice;
    if (ImGui::Combo("Devices", &this->selectedDevice, devices, IM_ARRAYSIZE(devices))) {
        if (_controller->getCurrentDeviceName() != "") {
            if (this->deviceNames[this->selectedDevice] != _controller->getCurrentDeviceName()) {
                for (int i = 0; i < this->deviceNames.size(); i++) {
                    if (this->deviceNames[i] == this->deviceNames[this->selectedDevice]) {
                        if (_controller->isDeviceOpen()) {
                            this->selectedDevice = previousDeviceIndex;
                            LogController::getInstance()->addLog(Utils::getTime(), "Other device is opened", LogType::ERROR);
                        } else {
                            _controller->setNewDeviceByName(this->deviceNames[i]);
                        }
                    }
                }
            }
        } else {
            _controller->setNewDeviceByName(this->deviceNames[this->selectedDevice]);
        }
    }
    ImGui::SameLine();
    if (_controller->getCurrentDeviceName() != "") {
        if (_controller->isDeviceOpen()) {
            if (ImGui::Button("Close Device", ImVec2(100.0f, 25.0f))) {
                _controller->setIsDeviceOpen(false);
                _controller->closeCurrentDevice();
            }
        } else {
            if (ImGui::Button("Open Device", ImVec2(100.0f, 25.0f))) {
                _controller->setIsDeviceOpen(true);
                _controller->openCurrentDevice();
            }
        }

    } else {
        ImGui::BeginDisabled();
        ImGui::Button("Open Device", ImVec2(100.0f, 25.0f));
        ImGui::EndDisabled();
    }

    if (_controller->getCurrentDeviceName() != "") {
        ImGui::Text("Info:");
        if (_controller->isDeviceOpen()) {
            ImGui::Text("Device Status: OPEN");
        } else {
            ImGui::Text("Device Status: CLOSE");
        }
        // ImGui::Text("Received packets: %ld", _model->getDeviceStats()[0]);
        // ImGui::Text("Droped packets: %ld", _model->getDeviceStats()[1]);
        // ImGui::Text("Droped packets by interface: %ld", _model->getDeviceStats()[2]);
        ImGui::Text("IPv4 address: %s", _model->getDeviceIPv4Address().c_str());
        ImGui::Text("IPv6 address: %s", _model->getDeviceIPv6Address().c_str());
        ImGui::Text("MAC address: %s", _model->getDeviceMacAddress().c_str());
        ImGui::Text("Default gateway: %s", _model->getDefaultGateway().c_str());
        ImGui::Text("MTU: %d", _model->getMtu());
        for (auto &server : _model->getDnsServers()) {
            ImGui::Text("DNS server: %s", server.c_str());
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
