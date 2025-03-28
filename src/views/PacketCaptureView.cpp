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
#include <IcmpLayer.h>
#include <IcmpV6Layer.h>
#include <IgmpLayer.h>
#include <TcpLayer.h>
#include <UdpLayer.h>

PacketCaptureView::PacketCaptureView() {
    this->windowTitle = "PACKETS";
    this->isWindowOpened = true;
}

void PacketCaptureView::draw(std::shared_ptr<MainController> controller, std::vector<CapturedPackets> &packets) {
    this->isDownKeyPressed = false;
    this->isUpKeyPressed = false;

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

        ImGuiListClipper clipper;
        clipper.Begin(controller->getPacketCapturedVectorSize());

        std::lock_guard<std::mutex> lock(guard_1);
        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                auto &packet = packets[i];
                this->keyboardHandling(controller, packets, packet);
                auto &pcapPacket = packet.packet;
                ImGui::TableNextRow();

                this->displayIndex(packet, controller);
                this->displayTime(pcapPacket);
                this->displayPacketSize(pcapPacket);
                this->displayDataLinkLayer(pcapPacket);
                this->displayNetworkProtocol(pcapPacket);
                this->displayTransportProtocol(pcapPacket);
                this->displayTransportLayer(pcapPacket);
            }
        }
        clipper.End();
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
    if (ImGui::Selectable(std::string("##selectableRow" + std::to_string(packet.id)).c_str(), packet.selected, this->selectableRowFlags))
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

void PacketCaptureView::displayDataLinkLayer(const pcpp::Packet &packet) {

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
    std::shared_ptr<pcpp::ArpLayer> arpLayer;
    std::shared_ptr<pcpp::IcmpLayer> icmpLayer;
    std::shared_ptr<pcpp::IcmpV6Layer> icmpV6Layer;
    std::shared_ptr<pcpp::IgmpLayer> igmpLayer;

    if (packet.isPacketOfType(pcpp::IPv4)) {
        ipv4Layer = std::make_shared<pcpp::IPv4Layer>(*packet.getLayerOfType<pcpp::IPv4Layer>());
    }
    if (packet.isPacketOfType(pcpp::IPv6)) {
        ipv6Layer = std::make_shared<pcpp::IPv6Layer>(*packet.getLayerOfType<pcpp::IPv6Layer>());
    }
    if (packet.isPacketOfType(pcpp::ARP)) {
        arpLayer = std::make_shared<pcpp::ArpLayer>(*packet.getLayerOfType<pcpp::ArpLayer>());
    }
    if (packet.isPacketOfType(pcpp::ICMP)) {
        icmpLayer = std::make_shared<pcpp::IcmpLayer>(*packet.getLayerOfType<pcpp::IcmpLayer>());
    }
    if (packet.isPacketOfType(pcpp::ICMPv6)) {
        icmpV6Layer = std::make_shared<pcpp::IcmpV6Layer>(*packet.getLayerOfType<pcpp::IcmpV6Layer>());
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
    } else if (arpLayer != nullptr) {
        ImGui::TableNextColumn();
        ImGui::Text("%s", arpLayer->getSenderIpAddr().toString().c_str());
        ImGui::TableNextColumn();
        ImGui::Text("%s", arpLayer->getTargetIpAddr().toString().c_str());
    } else if (icmpLayer != nullptr) {
        ImGui::TableNextColumn();
        ImGui::TableNextColumn();
    } else if (icmpV6Layer != nullptr) {
        ImGui::TableNextColumn();
        ImGui::TableNextColumn();
    } else if (igmpLayer != nullptr) {
        ImGui::TableNextColumn();
        ImGui::TableNextColumn();
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
    } else if (packet.isPacketOfType(pcpp::ARP)) {
        ImGui::Text("%s", "ARP");
    } else if (packet.isPacketOfType(pcpp::ICMP)) {
        ImGui::Text("%s", "ICMP");
    } else if (packet.isPacketOfType(pcpp::ICMPv6)) {
        ImGui::Text("%s", "ICMPv6");
    } else if (packet.isPacketOfType(pcpp::HTTP)) {
        ImGui::Text("%s", "HTTP");
    } else if (packet.isPacketOfType(pcpp::FTP)) {
        ImGui::Text("%s", "FTP");
    } else if (packet.isPacketOfType(pcpp::SSH)) {
        ImGui::Text("%s", "SSH");
    } else if (packet.isPacketOfType(pcpp::Telnet)) {
        ImGui::Text("%s", "Telnet");
    } else if (packet.isPacketOfType(pcpp::DNS)) {
        ImGui::Text("%s", "DNS");
    } else if (packet.isPacketOfType(pcpp::DHCP)) {
        ImGui::Text("%s", "DHCP");
    } else if (packet.isPacketOfType(pcpp::DHCPv6)) {
        ImGui::Text("%s", "DHCPv6");
    } else if (packet.isPacketOfType(pcpp::IGMP)) {
        ImGui::Text("%s", "IGMP");
    } else if (packet.isPacketOfType(pcpp::IGMPv1)) {
        ImGui::Text("%s", "IGMPv1");
    } else if (packet.isPacketOfType(pcpp::IGMPv2)) {
        ImGui::Text("%s", "IGMPv2");
    } else if (packet.isPacketOfType(pcpp::IGMPv3)) {
        ImGui::Text("%s", "IGMPv3");
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
    if (ImGui::IsKeyPressed(ImGuiKey_Enter)) {
        if (!controller->isDisplayedPakcet() && packet.selected) {
            this->selectedRow = packet.id;
            controller->setIsDisplayedPakcet(true);
        }
    }
    // if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
    //     if (!controller->isDisplayedPakcet() && packet.selected) {
    //         this->selectedRow = packet.id;
    //         controller->setIsDisplayedPakcet(true);
    //     }
    // }
    if (packet.selected && !controller->isDisplayedPakcet()) {
        if (ImGui::IsKeyPressed((ImGuiKey_DownArrow)) && !isDownKeyPressed) {
            if (packet.id + 1 < size) {
                packet.selected = false;
                packets[packet.id + 1].selected = true;
                isDownKeyPressed = true;

            }
        }
        if (ImGui::IsKeyPressed((ImGuiKey_UpArrow)) && !isUpKeyPressed) {
            if (packet.id - 1 >= 0) {
                packet.selected = false;
                packets[packet.id - 1].selected = true;
                isUpKeyPressed = true;
            }
        }
    }
    // if (packet.selected && !controller->isDisplayedPakcet()) {
    //     if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow)) && !isDownKeyPressed) {
    //         if (packet.id + 1 < size) {
    //             packet.selected = false;
    //             packets[packet.id + 1].selected = true;
    //             isDownKeyPressed = true;
    //
    //         }
    //     }
    //     if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow)) && !isUpKeyPressed) {
    //         if (packet.id - 1 >= 0) {
    //             packet.selected = false;
    //             packets[packet.id - 1].selected = true;
    //             isUpKeyPressed = true;
    //         }
    //     }
    // }
}



