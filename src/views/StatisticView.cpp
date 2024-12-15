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
        // Get the current time
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        std::stringstream ss;
        ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S.%f"); // Or other format as needed
        std::string currentTimestamp = ss.str();


        controller->updateData(currentTimestamp);

        // Accessing the last element in deques to set the histogram values.
        // Use .back().second to get the count (the .second part of the pair)
        if (!controller->arpData.empty()) {
            this->histogramData[0] = static_cast<float>(controller->arpData.back().second);
        } // if data is empty - it should likely stay unchanged, which is implicitly handled by this `if` statement
        if (!controller->imcpv4Data.empty()) {
            this->histogramData[1] = static_cast<float>(controller->imcpv4Data.back().second);
        }
        // Repeat the pattern for the remaining entries in histogramData and corresponding data in the controller.
        if (!controller->ipv4Data.empty()) {
            this->histogramData[2] = static_cast<float>(controller->ipv4Data.back().second);
        }
        if (!controller->ipv6Data.empty()) {
            this->histogramData[3] = static_cast<float>(controller->ipv6Data.back().second);
        }
        if (!controller->tcpData.empty()) {
            this->histogramData[4] = static_cast<float>(controller->tcpData.back().second);
        }
        if (!controller->udpData.empty()) {
            this->histogramData[5] = static_cast<float>(controller->udpData.back().second);
        }
        if (!controller->dhcpv4Data.empty()) {
            this->histogramData[6] = static_cast<float>(controller->dhcpv4Data.back().second);
        }
        if (!controller->dnsData.empty()) {
            this->histogramData[7] = static_cast<float>(controller->dnsData.back().second);
        }
        if (!controller->ftpData.empty()) {
            this->histogramData[8] = static_cast<float>(controller->ftpData.back().second);
        }
        if (!controller->httpData.empty()) {
            this->histogramData[9] = static_cast<float>(controller->httpData.back().second);
        }
        if (!controller->sshData.empty()) {
            this->histogramData[10] = static_cast<float>(controller->sshData.back().second);
        }
        if (!controller->telnetData.empty()) {
            this->histogramData[11] = static_cast<float>(controller->telnetData.back().second);
        }
        ImGui::Text("TCP: %d", controller->getModel()->tcpCount);

        if (!controller->getTcpData().empty()) {
            auto tcpData = controller->getTcpData(); // getDequeAsVector returns vector<pair<float, float>> now.

            // Separate the time and count data
            std::vector<float> timeData;
            std::vector<float> countData;
            timeData.reserve(tcpData.size());
            countData.reserve(tcpData.size());

            for (const auto& p : tcpData) {
                timeData.push_back(p.first);
                countData.push_back(p.second);
            }

            // Find the maximum count for setting the plot scale
            float maxCount = 0.f;
            if(!countData.empty())
                maxCount = *std::max_element(countData.begin(), countData.end());

            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[1]);
            // Important: provide countData, not timeData as a value array
            ImGui::PlotLines("TCP", countData.data(), countData.size(), 0, nullptr, 0.0f, maxCount, ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No TCP Data available");
        }
        ImGui::Text("UDP: %d", controller->getModel()->udpCount);

        if (!controller->getUdpData().empty()) {
            auto udpData = controller->getUdpData(); // getDequeAsVector returns vector<pair<float, float>> now.

            // Separate the time and count data
            std::vector<float> timeData;
            std::vector<float> countData;
            timeData.reserve(udpData.size());
            countData.reserve(udpData.size());

            for (const auto& p : udpData) {
                timeData.push_back(p.first);
                countData.push_back(p.second);
            }

            // Find the maximum count for setting the plot scale
            float maxCount = 0.f;
            if(!countData.empty())
                maxCount = *std::max_element(countData.begin(), countData.end());

            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[2]);
            // Important: provide countData, not timeData as a value array
            ImGui::PlotLines("UDP", countData.data(), countData.size(), 0, nullptr, 0.0f, maxCount, ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No UDP Data available");
        }
        ImGui::Text("ICMP: %d", controller->getModel()->imcpv4Count);

        if (!controller->getImcpv4Data().empty()) {
            auto icmpData = controller->getImcpv4Data(); // getDequeAsVector returns vector<pair<float, float>> now.

            // Separate the time and count data
            std::vector<float> timeData;
            std::vector<float> countData;
            timeData.reserve(icmpData.size());
            countData.reserve(icmpData.size());

            for (const auto& p : icmpData) {
                timeData.push_back(p.first);
                countData.push_back(p.second);
            }

            // Find the maximum count for setting the plot scale
            float maxCount = 0.f;
            if(!countData.empty())
                maxCount = *std::max_element(countData.begin(), countData.end());

            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[3]);
            // Important: provide countData, not timeData as a value array
            ImGui::PlotLines("ICMP", countData.data(), countData.size(), 0, nullptr, 0.0f, maxCount, ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No ICMP Data available");
        }
        ImGui::NextColumn();
        ImGui::Text("FTP: %d", controller->getModel()->ftpCount);

        if (!controller->getFtpData().empty()) {
            auto ftpData = controller->getFtpData(); // getDequeAsVector returns vector<pair<float, float>> now.

            // Separate the time and count data
            std::vector<float> timeData;
            std::vector<float> countData;
            timeData.reserve(ftpData.size());
            countData.reserve(ftpData.size());

            for (const auto& p : ftpData) {
                timeData.push_back(p.first);
                countData.push_back(p.second);
            }

            // Find the maximum count for setting the plot scale
            float maxCount = 0.f;
            if(!countData.empty())
                maxCount = *std::max_element(countData.begin(), countData.end());

            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[4]);
            // Important: provide countData, not timeData as a value array
            ImGui::PlotLines("FTP", countData.data(), countData.size(), 0, nullptr, 0.0f, maxCount, ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No FTP Data available");
        }
        ImGui::Text("TELNET: %d", controller->getModel()->telnetCount);

        if (!controller->getTelnetData().empty()) {
            auto telnetData = controller->getTelnetData(); // getDequeAsVector returns vector<pair<float, float>> now.

            // Separate the time and count data
            std::vector<float> timeData;
            std::vector<float> countData;
            timeData.reserve(telnetData.size());
            countData.reserve(telnetData.size());

            for (const auto& p : telnetData) {
                timeData.push_back(p.first);
                countData.push_back(p.second);
            }

            // Find the maximum count for setting the plot scale
            float maxCount = 0.f;
            if(!countData.empty())
                maxCount = *std::max_element(countData.begin(), countData.end());

            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[5]);
            // Important: provide countData, not timeData as a value array
            ImGui::PlotLines("TELNET", countData.data(), countData.size(), 0, nullptr, 0.0f, maxCount, ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No TELNET Data available");
        }

        ImGui::Text("ARP: %d", controller->getModel()->arpCount);

        if (!controller->getTelnetData().empty()) {
            auto arpData = controller->getArpData(); // getDequeAsVector returns vector<pair<float, float>> now.

            // Separate the time and count data
            std::vector<float> timeData;
            std::vector<float> countData;
            timeData.reserve(arpData.size());
            countData.reserve(arpData.size());

            for (const auto& p : arpData) {
                timeData.push_back(p.first);
                countData.push_back(p.second);
            }

            // Find the maximum count for setting the plot scale
            float maxCount = 0.f;
            if(!countData.empty())
                maxCount = *std::max_element(countData.begin(), countData.end());

            ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[6]);
            // Important: provide countData, not timeData as a value array
            ImGui::PlotLines("ARP", countData.data(), countData.size(), 0, nullptr, 0.0f, maxCount, ImVec2(350, 50.0f));
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("No ARP Data available");
        }
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
        // ImGui::Text("ARP: %d", controller->getModel()->arpCount);
        // if (!controller->getArpData().empty()) {
        //     const std::vector<float>& data = controller->getArpData();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[1]);
        //     ImGui::PlotLines("ARP", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No ARP Data available");
        // }
        // ImGui::Text("ICMPv4: %d", controller->getModel()->imcpv4Count);
        // if (!controller->getImcpv4Data().empty()) {
        //     const std::vector<float>& data = controller->getImcpv4Data();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[2]);
        //     ImGui::PlotLines("ICMPv4", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No ICMPv4 Data available");
        // }
        // ImGui::Text("IPv4: %d", controller->getModel()->ipv4Count);
        // if (!controller->getIpv4Data().empty()) {
        //     const std::vector<float>& data = controller->getIpv4Data();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[3]);
        //     ImGui::PlotLines("IPv4", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No IPv4 Data available");
        // }
        // ImGui::Text("IPv6: %d", controller->getModel()->ipv6Count);
        // if (!controller->getIpv6Data().empty()) {
        //     const std::vector<float>& data = controller->getIpv6Data();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[4]);
        //     ImGui::PlotLines("IPv6", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No IPv6 Data available");
        // }
        //
        // ImGui::Text("UDP: %d", controller->getModel()->udpCount);
        // if (!controller->getUdpData().empty()) {
        //     const std::vector<float>& data = controller->getUdpData();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[6]);
        //     ImGui::PlotLines("UDP", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No UDP Data available");
        // }
        // ImGui::NextColumn();
        // ImGui::Text("DHCPv4: %d", controller->getModel()->dhcpv4Count);
        // if (!controller->getDhcpv4Data().empty()) {
        //     const std::vector<float>& data = controller->getDhcpv4Data();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
        //     ImGui::PlotLines("DHCPv4", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No DHCPv4 Data available");
        // }
        // ImGui::Text("DNS: %d", controller->getModel()->dnsCount);
        // if (!controller->getDhcpv4Data().empty()) {
        //     const std::vector<float>& data = controller->getDnsData();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
        //     ImGui::PlotLines("DNS", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No DNS Data available");
        // }
        // ImGui::Text("FTP: %d", controller->getModel()->ftpCount);
        // if (!controller->getDhcpv4Data().empty()) {
        //     const std::vector<float>& data = controller->getFtpData();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
        //     ImGui::PlotLines("FTP", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No FTP Data available");
        // }
        // ImGui::Text("HTTP: %d", controller->getModel()->httpCount);
        // if (!controller->getDhcpv4Data().empty()) {
        //     const std::vector<float>& data = controller->getHttpData();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
        //     ImGui::PlotLines("HTTP", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No HTTP Data available");
        // }
        // // ImGui::Text("SMTP: %d", controller->getModel()->smtpCount);
        // // if (!controller->getDhcpv4Data().empty()) {
        // //     const std::vector<float>& data = controller->getSmtpData();
        // //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
        // //     ImGui::PlotLines("SMTP", data.data(), data.size(), 0, nullptr,
        // //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        // //     ImGui::PopStyleColor();
        // // } else {
        // //     ImGui::Text("No SMTP Data available");
        // // }
        // ImGui::Text("SSH: %d", controller->getModel()->sshCount);
        // if (!controller->getDhcpv4Data().empty()) {
        //     const std::vector<float>& data = controller->getSshData();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
        //     ImGui::PlotLines("SSH", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No SSH Data available");
        // }
        // ImGui::Text("Telnet: %d", controller->getModel()->telnetCount);
        // if (!controller->getDhcpv4Data().empty()) {
        //     const std::vector<float>& data = controller->getTelnetData();
        //     ImGui::PushStyleColor(ImGuiCol_PlotLines, colors[7]);
        //     ImGui::PlotLines("Telnet", data.data(), data.size(), 0, nullptr,
        //         0.0f, *max_element(data.begin(), data.end()), ImVec2(350, 50.0f));
        //     ImGui::PopStyleColor();
        // } else {
        //     ImGui::Text("No Telnet Data available");
        // }
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

