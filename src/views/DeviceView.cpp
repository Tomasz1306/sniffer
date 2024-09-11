//
// Created by tomasz on 9/1/24.
//

#include "views/DeviceView.h"

DeviceView::DeviceView() {

}

void DeviceView::draw(std::shared_ptr<DeviceController> _controller, std::shared_ptr<DeviceModel> _model) {
    //TODO przeniesc packeWindowInitialized z globals do tej instacji jako pole
    ImGui::SetWindowSize("DEVICES", ImVec2(1200.0f, 200.0f));
    if (!packetWindowInitialized) {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        packetWindowInitialized = true;
    }

    ImGui::Begin("DEVICES", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);


    ImGui::End();
}

