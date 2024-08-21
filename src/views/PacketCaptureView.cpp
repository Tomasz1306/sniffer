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

void PacketCaptureView::draw(std::vector<CapturedPackets> &packets) {
    //TODO przeniesc packeWindowInitialized z globals do tej instacji jako pole
    ImGui::SetWindowSize("Okno", ImVec2(1200.0f, 500.0f));
    if (!packetWindowInitialized) {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 520.0f));
        packetWindowInitialized = true;
    }

    ImGui::Begin("Okno", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open")) {  }
            if (ImGui::MenuItem("Save")) { }
            if (ImGui::MenuItem("Quit")) { }
            ImGui::EndMenu();
        }


        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo")) {  }
            if (ImGui::MenuItem("Redo")) {  }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    this->displayTableOfPackets(packets);


    ImGui::End();
}

void PacketCaptureView::displayTableOfPackets(std::vector<CapturedPackets> &packets) {
    if (ImGui::BeginTable("Packets", 10, ImGuiTableFlags_ScrollY | ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable)) {
        ImGui::TableSetupScrollFreeze(0,1);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("TIME", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("LENGTH", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("MAC SRC", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("MAC DST", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("IP SRC", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("IP DST", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("PROTO", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("SRC PORT", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("DST PORT", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();


        std::lock_guard<std::mutex> lock(guard_1);
        for (auto& packet : packets) {
            auto &pcapPacket = packet.packet;
            ImGui::TableNextRow();

            this->displayIndex(packet);
            this->displayTime(pcapPacket);
            this->displayPacketSize(pcapPacket);
            this->displayEthernetLayer(pcapPacket);
            this->displayNetworkProtocol(pcapPacket);
            this->displayTransportProtocol(pcapPacket);
            this->displayTransportLayer(pcapPacket);

        }
        ImGui::EndTable();
    }
}


void PacketCaptureView::displayIndex(CapturedPackets &packet) {
    ImGui::TableNextColumn();

    if (ImGui::Selectable("##selectableRow", packet.selected, this->selectableRowFlags))
    {
        packet.selected = !packet.selected;
    }
    ImGui::SameLine();
    ImGui::Text("%d", packet.id);
}


void PacketCaptureView::displayTime(const pcpp::Packet &packet) {
    auto time = this->parseTimeToStr(packet.getRawPacket()->getPacketTimeStamp().tv_sec);
    ImGui::TableNextColumn();
    ImGui::Text("%s", time.c_str());
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
    std::shared_ptr<pcpp::TcpLayer> tcpLayer;
    std::shared_ptr<pcpp::UdpLayer> udpLayer;

    if (packet.isPacketOfType(pcpp::TCP)) {
        tcpLayer = std::make_shared<pcpp::TcpLayer>(*packet.getLayerOfType<pcpp::TcpLayer>());
        int srcPort = tcpLayer->getSrcPort();
        int dstPort = tcpLayer->getDstPort();
        ImGui::TableNextColumn();
        ImGui::Text("%d", srcPort);
        ImGui::TableNextColumn();
        ImGui::Text("%d", dstPort);
    } else if (packet.isPacketOfType(pcpp::UDP)) {
        udpLayer = std::make_shared<pcpp::UdpLayer>(*packet.getLayerOfType<pcpp::UdpLayer>());
        int srcPort = udpLayer->getSrcPort();
        int dstPort = udpLayer->getDstPort();
        ImGui::TableNextColumn();
        ImGui::Text("%d", srcPort);
        ImGui::TableNextColumn();
        ImGui::Text("%d", dstPort);
    } else {
        ImGui::TableNextColumn();
        ImGui::Text("%d", 0);
        ImGui::TableNextColumn();
        ImGui::Text("%d", 0);
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




