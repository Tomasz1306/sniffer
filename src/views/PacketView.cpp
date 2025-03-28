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

PacketView::PacketView() {
    this->windowTitle = "PACKET";
    this->windowHeight = 800.0f;
    this->windowWidth = 800.0f;
    this->windowX = 600.0f;
    this->windowY = 50.0f;
    this->isWindowOpened = false;
    this->windowFlags = 0;
}

std::string PacketView::byteToHex(unsigned char byte) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)byte;
    return ss.str();
}

void PacketView::draw(std::shared_ptr<MainController> controller, CapturedPackets &_packet) {
    ImGui::SetWindowSize(this->windowTitle.c_str(), ImVec2(this->windowHeight, this->windowWidth));
    if (!this->isWindowInitialized) {
        ImGui::SetNextWindowPos(ImVec2(this->windowX, this->windowY));
        this->isWindowInitialized = true;
    }
    this->keyboardHandling(controller, _packet);
    ImGui::Begin(this->windowTitle.c_str(), controller->getIsPacketDisplayed(), this->windowFlags);
    if (ImGui::Button("Close", ImVec2(980, 25))) {
        controller->setIsDisplayedPakcet(false);
        controller->setCurrentPacketId(-1);
    }
    if (!ImGui::IsWindowFocused())
    {
        controller->setIsDisplayedPakcet(false);
        controller->setCurrentPacketId(-1);
    }

    std::vector<std::string> info;
    _packet.packet.toStringList(info);
    for (auto &layer : info) {
        if (ImGui::Selectable(layer.c_str())) {
            ImGui::SetClipboardText(layer.c_str());
        }
    }

    ImGui::Text("Packet ID: %d", _packet.id);
    pcpp::PayloadLayer* payloadLayer = _packet.packet.getLayerOfType<pcpp::PayloadLayer>();
    if (payloadLayer !=nullptr) {
        const uint8_t *payload = _packet.packet.getLayerOfType<pcpp::PayloadLayer>()->getPayload();
        int payloadSize = _packet.packet.getLayerOfType<pcpp::PayloadLayer>()->getPayloadLen();
        std::string hexAndAsciiLine;
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
                for (int j = line_length; j < 16; ++j) {
                    hexStream << "   ";
                }
                hexAndAsciiLine += hexStream.str() + "     " + asciiStream.str() + '\n';
            }
            const size_t bufferSize = 256;
            static char buffer[bufferSize];
            strncpy(buffer, hexAndAsciiLine.c_str(), bufferSize);
            buffer[bufferSize - 1] = '\0';
            ImVec4 originalBackgroundColor = ImGui::GetStyle().Colors[ImGuiCol_FrameBg];
            ImVec4 originalBorderColor = ImGui::GetStyle().Colors[ImGuiCol_Border];
            ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = ImVec4(0, 0, 0, 0);
            ImGui::GetStyle().Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
            ImGui::InputTextMultiline("##payloadMultiLine", buffer, bufferSize, ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 6), ImGuiInputTextFlags_ReadOnly);
            ImGui::GetStyle().Colors[ImGuiCol_FrameBg] = originalBackgroundColor;
            ImGui::GetStyle().Colors[ImGuiCol_Border] = originalBorderColor;
        }
    }
    ImGui::End();
}

void PacketView::keyboardHandling(std::shared_ptr<MainController> controller, CapturedPackets &_packet) {
    if (ImGui::IsKeyPressed(ImGuiKey_RightArrow)) {
        controller->displayNextPacket();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow)) {
        controller->displayPrevPacket();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_DownArrow)) {
        controller->displayNextPacket();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_UpArrow)) {
        controller->displayPrevPacket();
    }
    if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
        controller->setIsDisplayedPakcet(false);
        controller->setCurrentPacketId(-1);
    }
}



