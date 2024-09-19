//
// Created by tomasz on 9/11/24.
//

#include "views/PacketView.h"
#include "imgui.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <PayloadLayer.h>

std::string PacketView::byteToHex(unsigned char byte) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    return ss.str();
}
//TODO store windows name to a field of class
//TODO store window flags to a fields of class
void PacketView::draw(std::shared_ptr<MainController> controller, CapturedPackets &_packet) {
    ImGui::SetWindowSize("PACKET", ImVec2(1000.0f, 800.0f));
    if (!isInitialized) {
        ImGui::SetNextWindowPos(ImVec2(200.0f, 200.0f));
        isInitialized = true;
    }
    this->keyboardHandling(controller, _packet);
    ImGui::Begin("PACKET");
    ImGui::Text("Packet");
    if (ImGui::Button("Close")) {
        controller->setIsDisplayedPakcet(false);
        controller->setCurrentPacketId(-1);
    }

    std::vector<std::string> info;
    _packet.packet.toStringList(info);
    for (auto &layer : info) {
        ImGui::BulletText("%s", layer.c_str());
    }

    ImGui::Text("Packet ID: %d", _packet.id);
    pcpp::PayloadLayer* payloadLayer = _packet.packet.getLayerOfType<pcpp::PayloadLayer>();
    if (payloadLayer !=nullptr) {
        const uint8_t *payload = _packet.packet.getLayerOfType<pcpp::PayloadLayer>()->getPayload();
        int payloadSize = _packet.packet.getLayerOfType<pcpp::PayloadLayer>()->getPayloadLen();
        if (payload) {
            ImGui::Text("Payload");

            for (int i = 0; i < payloadSize; i += 16) {
                std::stringstream hexStream;
                std::stringstream asciiStream;

                int line_length = std::min(16, payloadSize - i);
                for (int j = 0; j < line_length; ++j) {
                    if (j > 0) {
                        hexStream << ' ';
                        asciiStream << ' ';
                    }
                    unsigned char byte = payload[i + j];
                    hexStream << byteToHex(byte);
                    asciiStream << (isprint(byte) ? static_cast<char>(byte) : '.');
                }

                // Dodać przestrzeń, jeśli linia jest krótsza niż 16 bajtów
                for (int j = line_length; j < 16; ++j) {
                    hexStream << "   "; // Dwa znaki hex + jedna spacja
                }

                ImGui::Text("%s    %s", hexStream.str().c_str(), asciiStream.str().c_str());
            }
        }
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

void PacketView::keyboardHandling(std::shared_ptr<MainController> controller, CapturedPackets &_packet) {
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow))) {
        controller->displayNextPacket();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow))) {
        controller->displayPrevPacket();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow))) {
        controller->displayNextPacket();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow))) {
        controller->displayPrevPacket();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
        controller->setIsDisplayedPakcet(false);
        controller->setCurrentPacketId(-1);
    }
}



