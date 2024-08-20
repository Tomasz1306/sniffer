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


    if (ImGui::BeginTable("Packets", 9, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable)) {
        ImGui::TableSetupScrollFreeze(0,1);
        ImGui::TableSetupColumn("TIME", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableSetupColumn("LENGTH", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("MAC SRC", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("MAC DST", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("IP SRC", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("IP DST", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("PROTO", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("SRC PORT", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("DST PORT", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();  // Dodaje nagłówki kolumn

        std::lock_guard<std::mutex> lock(guard_1);
        for (auto& packet : packets) {
            ImGui::TableNextRow();


            const pcpp::HttpRequestLayer* httpRequestLayer = nullptr;
            const pcpp::HttpResponseLayer* httpResponseLayer = nullptr;

            this->displayTime(packet);

            this->displayPacketSize(packet);

            this->displayEthernetLayer(packet);

            this->displayNetworkProtocol(packet);

            this->displayTransportProtocol(packet);

            // if (packet.isPacketOfType(pcpp::HTTPRequest)) {
            //     httpRequestLayer = packet.getLayerOfType<pcpp::HttpRequestLayer>();
            // }
            // if (packet.isPacketOfType(pcpp::HTTPResponse)) {
            //     httpResponseLayer = packet.getLayerOfType<pcpp::HttpResponseLayer>();
            // }



            // if (httpRequestLayer != nullptr) {
            //     ImGui::TableNextColumn();
            //     ImGui::Text("HTTP");
            // } else if (httpResponseLayer != nullptr) {
            //     ImGui::TableNextColumn();
            //     ImGui::Text("HTTP");
            // } else if (tcpLayer != nullptr) {
            //     ImGui::TableNextColumn();
            //     ImGui::Text("TCP");
            // } else if (udpLayer != nullptr) {
            //     ImGui::TableNextColumn();
            //     ImGui::Text("UDP");
            // } else {
            //     ImGui::TableNextColumn();
            // }
        }
        ImGui::EndTable();
    }
    ImGui::End();
}

void PacketCaptureView::displayTime(const pcpp::Packet &packet) {
    auto time = this->parseTimeToStr(packet.getRawPacket()->getPacketTimeStamp().tv_sec);
    ImGui::TableNextColumn();
    ImGui::Text("%s", time.c);
}

void PacketCaptureView::displayPacketSize(const pcpp::Packet &packet) {
    auto rawPacket = packet.getRawPacketReadOnly();
    auto frameLength = rawPacket->getFrameLength();
    ImGui::TableNextColumn();
    ImGui::Text("%d", frameLength);
}

void PacketCaptureView::displayEthernetLayer(const pcpp::Packet &packet) {

    std::shared_ptr<pcpp::EthLayer> ethLayer;

    if (packet.isPacketOfType(pcpp::Ethernet)) {
        ethLayer = std::make_shared<pcpp::EthLayer>(*packet.getLayerOfType<pcpp::EthLayer>());
    }

    if (ethLayer != nullptr) {
        ImGui::TableNextColumn();
        ImGui::Text("%s", ethLayer->getSourceMac().toString().c_str());
        ImGui::TableNextColumn();
        ImGui::Text("%s", ethLayer->getDestMac().toString().c_str());
    }else {
        ImGui::TableNextColumn();
        ImGui::TableNextColumn();
    }
}

void PacketCaptureView::displayNetworkProtocol(const pcpp::Packet &packet) {
    std::shared_ptr<pcpp::IPv4Layer> ipv4Layer;
    std::shared_ptr<pcpp::IPv6Layer> ipv6Layer;

    if (packet.isPacketOfType(pcpp::IPv4)) {
        ipv4Layer = std::make_shared<pcpp::IPv4Layer>(*packet.getLayerOfType<pcpp::IPv4Layer>());
    }

    if (packet.isPacketOfType(pcpp::IPv6)) {
        ipv6Layer = std::make_shared<pcpp::IPv6Layer>(*packet.getLayerOfType<pcpp::IPv6Layer>());
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

void PacketCaptureView::displayTransportProtocol(const pcpp::Packet &packet) {
    ImGui::TableNextColumn();
    if (packet.isPacketOfType(pcpp::TCP)) {
        ImGui::Text("%s", "TCP");
    } else if (packet.isPacketOfType(pcpp::UDP)) {
        ImGui::Text("%s", "UDP");
    } else {
        ImGui::Text("%s", "UNK");
    }
}

void PacketCaptureView::displayTransportLayer(const pcpp::Packet &packet) {
    const pcpp::TcpLayer* tcpLayer = nullptr;
    const pcpp::UdpLayer* udpLayer = nullptr;

    if (packet.isPacketOfType(pcpp::TCP)) {
        tcpLayer = packet.getLayerOfType<pcpp::TcpLayer>();
    }

    if (packet.isPacketOfType(pcpp::UDP)) {
        udpLayer = packet.getLayerOfType<pcpp::UdpLayer>();
    }
}

void PacketCaptureView::displayFlags(const pcpp::Packet &packet) {

}

std::string PacketCaptureView::parseTimeToStr(std::time_t seconds) {

    const std::shared_ptr<tm> ptm = std::make_shared<tm>(*std::localtime(&seconds));
    char timeString[32] = "\0";
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", ptm.get());
    return Utils::formatTimestamp(&seconds);
}




