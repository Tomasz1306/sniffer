//
// Created by tomasz on 01.10.24.
//

#include "views/StatisticView.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "implot.h"

StatisticView::StatisticView() {
    this->windowTitle = "STATISTICS";
    this->isWindowOpened = true;
    this->histogramData.resize(14, 0.0f);
    this->currentHistogramData.resize(14, 0.0f);
    this->interpolationSpeed = 0.01f;
}

float lerp(float start, float end, float t) {
    return start + t * (end - start);
}

void generateSineWave(std::vector<float>& data, float frequency, float amplitude, float phase, int sampleCount) {
    data.clear();
    data.reserve(sampleCount);
    for (int i = 0; i < sampleCount; ++i) {
        data.push_back(amplitude * std::sin(frequency * i + phase));
    }
}

void generateSawtoothWave(std::vector<float>& data, float frequency, float amplitude, float phase, int sampleCount) {
    data.clear();
    data.reserve(sampleCount);
    for (int i = 0; i < sampleCount; ++i) {
        data.push_back(amplitude * (2.0f * (i * frequency - std::floor(i * frequency)) - 1.0f));
    }
}

void StatisticView::draw(std::shared_ptr<StatisticController> controller) {
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);
    ImGui::Text("Session Data: %s", controller->getModel()->date.c_str());
    ImGui::Columns(2, nullptr, false);

    for (int i = 0; i < this->histogramData.size(); i++) {
        this->histogramData[i] = 0.0f;
    }
    ImVec4 colors[] = { ImVec4(1, 0, 0, 1), ImVec4(0, 1, 0, 1), ImVec4(0, 0, 1, 1),
                        ImVec4(1, 1, 0, 1), ImVec4(1, 0, 1, 1), ImVec4(0, 1, 1, 1),
                        ImVec4(0.5, 0.5, 0.5, 1), ImVec4(0.6, 0.2, 0.2, 1) };

    if (controller->isDeviceSelected()) {
        controller->updateData();

        //this->histogramData[0] = static_cast<float>(controller->getModel()->ethernetCount);
        this->histogramData[0] = static_cast<float>(controller->getModel()->arpCount);
        this->histogramData[1] = static_cast<float>(controller->getModel()->imcpv4Count);
        this->histogramData[2] = static_cast<float>(controller->getModel()->ipv4Count);
        this->histogramData[3] = static_cast<float>(controller->getModel()->ipv6Count);
        this->histogramData[4] = static_cast<float>(controller->getModel()->tcpCount);
        this->histogramData[5] = static_cast<float>(controller->getModel()->udpCount);
        this->histogramData[6] = static_cast<float>(controller->getModel()->dhcpv4Count);
        this->histogramData[7] = static_cast<float>(controller->getModel()->dnsCount);
        this->histogramData[8] = static_cast<float>(controller->getModel()->ftpCount);
        this->histogramData[9] = static_cast<float>(controller->getModel()->httpCount);
        this->histogramData[10] = static_cast<float>(controller->getModel()->sshCount);
        this->histogramData[11] = static_cast<float>(controller->getModel()->telnetCount);
        // ImGui::Text("Ethernet: %d", controller->getModel()->ethernetCount);
        // if (!controller->getEthernetData().empty()) {
        //     const std::vector<float>& data = controller->getEthernetData();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[0]);
        //     ImGui::PlotLines("Ethernet", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No Ethernet Data available");
        // }
        ImGui::Text("ARP: %d", controller->getModel()->arpCount);
        if (!controller->getArpData().empty()) {
            const std::vector<float>& data = controller->getArpData();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[1]);
            ImGui::PlotLines("ARP", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No ARP Data available");
        }
        ImGui::Text("ICMPv4: %d", controller->getModel()->imcpv4Count);
        if (!controller->getImcpv4Data().empty()) {
            const std::vector<float>& data = controller->getImcpv4Data();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[2]);
            ImGui::PlotLines("ICMPv4", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No ICMPv4 Data available");
        }
        ImGui::Text("IPv4: %d", controller->getModel()->ipv4Count);
        if (!controller->getIpv4Data().empty()) {
            const std::vector<float>& data = controller->getIpv4Data();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[3]);
            ImGui::PlotLines("IPv4", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No IPv4 Data available");
        }
        ImGui::Text("IPv6: %d", controller->getModel()->ipv6Count);
        if (!controller->getIpv6Data().empty()) {
            const std::vector<float>& data = controller->getIpv6Data();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[4]);
            ImGui::PlotLines("IPv6", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No IPv6 Data available");
        }
        ImGui::Text("TCP: %d", controller->getModel()->tcpCount);
        if (!controller->getTcpData().empty()) {
            const std::vector<float>& data = controller->getTcpData();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[5]);
            ImGui::PlotLines("TCP", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No TCP Data available");
        }
        ImGui::Text("UDP: %d", controller->getModel()->udpCount);
        if (!controller->getUdpData().empty()) {
            const std::vector<float>& data = controller->getUdpData();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[6]);
            ImGui::PlotLines("UDP", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No UDP Data available");
        }
        ImGui::NextColumn();
        ImGui::Text("DHCPv4: %d", controller->getModel()->dhcpv4Count);
        if (!controller->getDhcpv4Data().empty()) {
            const std::vector<float>& data = controller->getDhcpv4Data();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
            ImGui::PlotLines("DHCPv4", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No DHCPv4 Data available");
        }
        ImGui::Text("DNS: %d", controller->getModel()->dnsCount);
        if (!controller->getDhcpv4Data().empty()) {
            const std::vector<float>& data = controller->getDnsData();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
            ImGui::PlotLines("DNS", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No DNS Data available");
        }
        ImGui::Text("FTP: %d", controller->getModel()->ftpCount);
        if (!controller->getDhcpv4Data().empty()) {
            const std::vector<float>& data = controller->getFtpData();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
            ImGui::PlotLines("FTP", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No FTP Data available");
        }
        ImGui::Text("HTTP: %d", controller->getModel()->httpCount);
        if (!controller->getDhcpv4Data().empty()) {
            const std::vector<float>& data = controller->getHttpData();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
            ImGui::PlotLines("HTTP", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No HTTP Data available");
        }
        // ImGui::Text("SMTP: %d", controller->getModel()->smtpCount);
        // if (!controller->getDhcpv4Data().empty()) {
        //     const std::vector<float>& data = controller->getSmtpData();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
        //     ImGui::PlotLines("SMTP", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No SMTP Data available");
        // }
        ImGui::Text("SSH: %d", controller->getModel()->sshCount);
        if (!controller->getDhcpv4Data().empty()) {
            const std::vector<float>& data = controller->getSshData();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
            ImGui::PlotLines("SSH", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No SSH Data available");
        }
        ImGui::Text("Telnet: %d", controller->getModel()->telnetCount);
        if (!controller->getDhcpv4Data().empty()) {
            const std::vector<float>& data = controller->getTelnetData();
            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
            ImGui::PlotLines("Telnet", data.data(), data.size(), 0, nullptr,
                0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No Telnet Data available");
        }
        ImGui::Columns(1);

        // Perform interpolation for smooth transitions
        for (size_t i = 0; i < histogramData.size(); ++i) {
            currentHistogramData[i] = lerp(currentHistogramData[i], histogramData[i], interpolationSpeed);
        }

        if (!histogramData.empty()) {
            // const char* labels[] = {"Ethernet", "ARP", "ICMP", "IPv4", "IPv6", "TCP", "UDP", "dhcp", "DNS", "FTP", "HTTP", "SMTP", "SSH", "Telnet"};
            const char* labels[] = {"ARP", "ICMP", "IPv4", "IPv6", "TCP", "UDP", "dhcp", "DNS", "FTP", "HTTP", "SMTP", "SSH", "Telnet"};

            if (ImPlot::BeginPlot("Histogram", ImVec2(600, 200))) {

                std::vector<int> intHistogram;
                for (int i = 0; i < currentHistogramData.size(); i++) {
                    intHistogram.push_back(static_cast<int>(currentHistogramData[i]));
                }

                ImPlot::SetupAxisLimits(ImAxis_X1, -50.0, currentHistogramData.size()); // Adjust X axis limits
                ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 12000.0);

                ImPlot::SetupAxis(ImAxis_X1, nullptr, ImPlotAxisFlags_NoTickLabels);

                ImPlot::PlotBarGroups(labels, intHistogram.data(), 13, 1, 100, 10);
                ImPlot::EndPlot();
            }
        } else {
            ImGui::Text("No data to display in the histogram.");
        }
    } else {
        ImGui::Text("Device is not selected");
    }
    ImGui::End();
}

