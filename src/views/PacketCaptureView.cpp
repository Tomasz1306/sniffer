#include "views/PacketCaptureView.h"
#include "global/Global.h"
#include "utils/Utils.h"

#include "imgui.h"

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>

#include <Device.h>
#include <Packet.h>
#include <RawPacket.h>
#include <IPv4Layer.h>
#include <IPv6Layer.h>
#include <EthLayer.h>
#include <TcpLayer.h>
#include <UdpLayer.h>
#include <HttpLayer.h>

void PacketCaptureView::draw(const std::vector<pcpp::Packet> &packets)
{
    //TODO przeniesc packeWindowInitialized z globals do tej instacji jako pole
    ImGui::SetWindowSize("Okno", ImVec2(1200.0f, 500.0f));
    if (!packetWindowInitialized) {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 520.0f));
        packetWindowInitialized = true;
    }

    ImGui::Begin("Okno", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);


    if (ImGui::BeginTable("Packets", 6, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable)) {
        ImGui::TableSetupScrollFreeze(0,1);
        ImGui::TableSetupColumn("TIME", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableSetupColumn("PROTOCOL", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("MAC SRC", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("MAC DST", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("IP SRC", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("IP DST", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();  // Dodaje nagłówki kolumn

        std::lock_guard<std::mutex> lock(guard_1);
        for (auto& packet : packets) {
            ImGui::TableNextRow();
            std::time_t seconds = packet.getRawPacket()->getPacketTimeStamp().tv_sec;

            const std::tm* ptm = std::localtime(&seconds);
            char timeString[32];
            std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", ptm);
            auto timestamp = Utils::formatTimestamp(&seconds);
            const pcpp::IPv4Layer* ipv4Layer = nullptr;
            const pcpp::IPv6Layer* ipv6Layer = nullptr;
            const pcpp::EthLayer* ethLayer = nullptr;
            const pcpp::TcpLayer* tcpLayer = nullptr;
            const pcpp::UdpLayer* udpLayer = nullptr;
            const pcpp::HttpRequestLayer* httpRequestLayer = nullptr;
            const pcpp::HttpResponseLayer* httpResponseLayer = nullptr;

            ImGui::TableNextColumn();
            ImGui::Text("%s", timeString);

            // Get the Ethernet layer
            if (packet.isPacketOfType(pcpp::Ethernet)) {
                ethLayer = packet.getLayerOfType<pcpp::EthLayer>();
            }

            // Get the IPv4 layer
            if (packet.isPacketOfType(pcpp::IPv4)) {
                ipv4Layer = packet.getLayerOfType<pcpp::IPv4Layer>();
            }

            // Get the IPv6 layer
            if (packet.isPacketOfType(pcpp::IPv6)) {
                ipv6Layer = packet.getLayerOfType<pcpp::IPv6Layer>();
            }

            if (packet.isPacketOfType(pcpp::HTTPRequest)) {
                httpRequestLayer = packet.getLayerOfType<pcpp::HttpRequestLayer>();
            }
            if (packet.isPacketOfType(pcpp::HTTPResponse)) {
                httpResponseLayer = packet.getLayerOfType<pcpp::HttpResponseLayer>();
            }

            if (packet.isPacketOfType(pcpp::TCP)) {
                tcpLayer = packet.getLayerOfType<pcpp::TcpLayer>();
            }

            if (packet.isPacketOfType(pcpp::UDP)) {
                udpLayer = packet.getLayerOfType<pcpp::UdpLayer>();
            }

            if (httpRequestLayer != nullptr) {
                ImGui::TableNextColumn();
                ImGui::Text("HTTP");
            } else if (httpResponseLayer != nullptr) {
                ImGui::TableNextColumn();
                ImGui::Text("HTTP");
            } else if (tcpLayer != nullptr) {
                ImGui::TableNextColumn();
                ImGui::Text("TCP");
            } else if (udpLayer != nullptr) {
                ImGui::TableNextColumn();
                ImGui::Text("UDP");
            } else {
                ImGui::TableNextColumn();
            }

            // Now, we can display the information in ImGui
            if (ethLayer != nullptr) {
                ImGui::TableNextColumn();
                ImGui::Text("%s", ethLayer->getSourceMac().toString().c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", ethLayer->getDestMac().toString().c_str());
            }else {
                ImGui::TableNextColumn();
                ImGui::TableNextColumn();
            }

            if (ipv4Layer != nullptr) {
                ImGui::TableNextColumn();
                ImGui::Text("%s", ipv4Layer->getSrcIPAddress().toString().c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", ipv4Layer->getDstIPAddress().toString().c_str());
            } else if (ipv6Layer != nullptr) {
                ImGui::TableNextColumn();
                ImGui::Text("%s", ipv6Layer->getSrcIPAddress().toString().c_str());
                ImGui::TableNextColumn();
                ImGui::Text("%s", ipv6Layer->getDstIPAddress().toString().c_str());
            } else {
                ImGui::TableNextColumn();
                ImGui::TableNextColumn();
            }
        }
        ImGui::EndTable();
    }
    ImGui::End();
}
