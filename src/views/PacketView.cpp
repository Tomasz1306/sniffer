//
// Created by tomasz on 9/11/24.
//

#include "views/PacketView.h"
#include "imgui.h"
#include <vector>
#include <string>
#include <iostream>

//TODO store windows name to a field of class
//TODO store window flags to a fields of class
void PacketView::draw(std::shared_ptr<MainController> controller, CapturedPackets &_packet) {
    ImGui::SetWindowSize("PACKET", ImVec2(800.0f, 800.0f));
    if (!isInitialized) {
        ImGui::SetNextWindowPos(ImVec2(200.0f, 200.0f));
        isInitialized = true;
    }
    ImGui::Begin("PACKET");
    ImGui::Text("Packet");
    if (ImGui::Button("Close")) {
        controller->setIsDisplayedPakcet(false);
        controller->setCurrentPacketId(-1);
    }
    std::vector<std::string> info;
    _packet.packet.toStringList(info);
    for (auto &layer : info) {
        ImGui::Text("%s", layer.c_str());
    }
    ImGui::End();
}

void PacketView::displayEthernetLayer(CapturedPackets &_packet) {

}

void PacketView::displayNetworkLayer(CapturedPackets &_packet) {

}

void PacketView::displayTransportLayer(CapturedPackets &_packet) {

}

void PacketView::displayApplicationLayer(CapturedPackets &_packet) {

}


