//
// Created by tomasz on 9/11/24.
//

#include "views/PacketView.h"
#include "imgui.h"
//TODO store windows name to a field of class
//TODO store window flags to a fields of class
void PacketView::draw(CapturedPackets &_packet) {
    ImGui::SetWindowSize("PACKET", ImVec2(500.0f, 500.0f));
    if (!isInitialized) {
        ImGui::SetNextWindowPos(ImVec2(500.0f, 500.0f));
        isInitialized = true;
    }
    ImGui::Begin("PACKET");
    ImGui::Text("Packet");
    ImGui::Button("Close");
    ImGui::End();
}