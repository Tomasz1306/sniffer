#include "views/PacketCaptureView.h"
#include "global/Global.h"
#include "utils/Utils.h"

#include "imgui.h"

#include <iostream>
#include <mutex>
#include <thread>

#include <Packet.h>
#include <RawPacket.h>
#include <IPv4Layer.h>
#include <IPv6Layer.h>
#include <EthLayer.h>
#include <TcpLayer.h>
#include <UdpLayer.h>

PacketCaptureView::PacketCaptureView() {
    this->windowTitle = "PACKETS";
    // this->windowHeight = 1200.0f;
    // this->windowWidth = 550.0f;
    // this->windowX = 0.0f;
    // this->windowY = 450.0f;
    this->isWindowOpened = true;
    // this->windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void PacketCaptureView::draw(std::shared_ptr<MainController> controller, std::vector<CapturedPackets> &packets) {
    this->isDownKeyPressed = false;
    this->isUpKeyPressed = false;
    // ImGui::SetWindowSize(this->windowTitle.c_str(), ImVec2(this->windowHeight, this->windowWidth));
    // if (!this->isWindowInitialized) {
    //     ImGui::SetNextWindowPos(ImVec2(this->windowX, this->windowY));
    //     this->isWindowInitialized = true;
    // }
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);

    this->displayMenuBar();
    this->displayOption(controller);
    this->displayTableOfPackets(packets, controller);

    ImGui::End();
}

void PacketCaptureView::displayMenuBar() {
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open")) {

            }
            if (ImGui::MenuItem("Save")) {

            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void PacketCaptureView::displayOption(std::shared_ptr<MainController> controller) {

    ImGui::BeginGroup();
    if (controller->isDeviceOpen()) {
        if (ImGui::Button(this->captureButtonText.c_str(), ImVec2(200, 20))) {
            if (this->captureButtonState == captureState::STOP) {
                this->captureButtonState = captureState::START;
                this->captureButtonText = "STOP";
                std::thread startCaptureThread([controller] () {
                    controller->startCapture();
                });

                startCaptureThread.detach();
            } else {
                this->captureButtonState = captureState::STOP;
                this->captureButtonText = "START";
                std::thread stopCaptureThread([controller]() {
                   controller->stopCapture();
                });
                stopCaptureThread.detach();
            }
        }
    } else {
        ImGui::BeginDisabled();
        ImGui::Button(this->captureButtonText.c_str(), ImVec2(200, 20));
        ImGui::EndDisabled();
    }

    ImGui::SameLine();
    if (controller->getPacketCapturedVectorSize() > 0) {
        if (ImGui::Button("CLEAR", ImVec2(200, 20))) {
            controller->clearTableOfPackets();
        }
    } else {
        ImGui::BeginDisabled();
        ImGui::Button("CLEAR", ImVec2(200, 20));
        ImGui::EndDisabled();
    }
    ImGui::EndGroup();
}

void PacketCaptureView::displayTableOfPackets(std::vector<CapturedPackets> &packets, std::shared_ptr<MainController> controller) {
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

        float scrollY = ImGui::GetScrollY();
        float scrollMaxY = ImGui::GetScrollMaxY();

        std::lock_guard<std::mutex> lock(guard_1);
        for (auto& packet : packets) {
            this->keyboardHandling(controller, packets, packet);
            auto &pcapPacket = packet.packet;
            ImGui::TableNextRow();

            this->displayIndex(packet, controller);
            this->displayTime(pcapPacket);
            this->displayPacketSize(pcapPacket);
            this->displayEthernetLayer(pcapPacket);
            this->displayNetworkProtocol(pcapPacket);
            this->displayTransportProtocol(pcapPacket);
            this->displayTransportLayer(pcapPacket);
        }
        if (this->autoScroll && scrollY >= scrollMaxY) {
            ImGui::SetScrollHereY(1.0f);
        }
        if (scrollY < scrollMaxY) {
            this->autoScroll = false;
        } else {
            this->autoScroll = true;
        }
        ImGui::EndTable();
    }
}


void PacketCaptureView::displayIndex(CapturedPackets &packet, std::shared_ptr<MainController> controller) {
    ImGui::TableNextColumn();

    if (this->singleSelectRow != -1 && this->singleSelectRow != packet.id) {
        packet.selected = false;
    }
    if (this->selectedRow == packet.id && packet.isOpen) {
        packet.selected = true;
    }
    if (this->selectedRow != -1 && this->selectedRow != packet.id && packet.selected) {
        packet.selected = false;
    }
    if (ImGui::Selectable("##selectableRow", packet.selected, this->selectableRowFlags))
    {
        packet.selected = !packet.selected;
        if (packet.selected) {
            this->singleSelectRow = packet.id;
        }
    }
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
        this->selectedRow = packet.id;
        this->singleSelectRow = packet.id;
        packet.isOpen = true;
        controller->setIsDisplayedPakcet(true);
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
    } else {
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
        ImGui::Text("%.11s...", ipv6Layer->getSrcIPAddress().toString().c_str());
        ImGui::TableNextColumn();
        ImGui::Text("%.11s...", ipv6Layer->getDstIPAddress().toString().c_str());
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

int PacketCaptureView::getCurrentSelectedPacketId() const {
    return this->selectedRow;
}

void PacketCaptureView::setCurrentSelectedPacketId(int id) {
    this->selectedRow = id;
}

void PacketCaptureView::keyboardHandling(std::shared_ptr<MainController> controller, std::vector<CapturedPackets> &packets, CapturedPackets &packet) {
    int size = packets.size();
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
        if (!controller->isDisplayedPakcet() && packet.selected) {
            this->selectedRow = packet.id;
            controller->setIsDisplayedPakcet(true);
        }
    }
    if (packet.selected && !controller->isDisplayedPakcet()) {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow)) && !isDownKeyPressed) {
            if (packet.id + 1 < size) {
                packet.selected = false;
                packets[packet.id + 1].selected = true;
                isDownKeyPressed = true;

            }
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow)) && !isUpKeyPressed) {
            if (packet.id - 1 >= 0) {
                packet.selected = false;
                packets[packet.id - 1].selected = true;
                isUpKeyPressed = true;
            }
        }
    }
}



