//
// Created by tomasz on 01.10.24.
//

#include "views/StatisticView.h"
#include "imgui.h"
#include "imgui_internal.h"

StatisticView::StatisticView() {
    this->windowTitle = "STATISTICS";
    this->isWindowOpened = true;
}

void StatisticView::draw(std::shared_ptr<StatisticController> controller) {
    // ImGui::SetWindowSize(this->windowTitle.c_str(), ImVec2(this->windowHeight, this->windowWidth));
    // if (!this->isWindowInitialized) {
    //     ImGui::SetNextWindowPos(ImVec2(this->windowX, this->windowY));
    //     this->isWindowInitialized = true;
    // }
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);

    ImGui::Text("Session Data: %s", controller->getModel()->date.c_str());

    ImGui::Columns(2, nullptr, false);
    if (controller->isDeviceSelected()) {

        ImGui::Text("Ethernet: %d", controller->getModel()->ethernetCount);
        ImGui::Text("ARP: %d", controller->getModel()->arpCount);
        ImGui::Text("ICMPv4: %d", controller->getModel()->imcpv4Count);
        ImGui::Text("ICMPv6: %d", controller->getModel()->imcpv6Count);
        ImGui::Text("IGMP: %d", controller->getModel()->igmpCount);
        ImGui::Text("IPv4: %d", controller->getModel()->ipv4Count);
        ImGui::Text("IPv6: %d", controller->getModel()->ipv6Count);
        ImGui::Text("TCP: %d", controller->getModel()->tcpCount);
        ImGui::Text("UDP: %d", controller->getModel()->udpCount);
        ImGui::Text("SDP: %d", controller->getModel()->sdpCount);
        ImGui::NextColumn();
        ImGui::Text("SIP: %d", controller->getModel()->sipCount);
        ImGui::Text("SSL/TSL: %d", controller->getModel()->sslTslCount);
        ImGui::Text("BGP: %d", controller->getModel()->bgpCount);
        ImGui::Text("DHCPv4: %d", controller->getModel()->dhcpv4Count);
        ImGui::Text("DHCPv6: %d", controller->getModel()->dhcpv6Count);
        ImGui::Text("DNS: %d", controller->getModel()->dnsCount);
        ImGui::Text("FTP: %d", controller->getModel()->ftpCount);
        ImGui::Text("HTTP: %d", controller->getModel()->httpCount);
        ImGui::Text("SMTP: %d", controller->getModel()->smtpCount);
        ImGui::Text("SSH: %d", controller->getModel()->sshCount);
        ImGui::Text("Telnet: %d", controller->getModel()->telnetCount);
        ImGui::Columns(1);
    } else {
        ImGui::Text("Device is not selected");
    }



    ImGui::End();
}