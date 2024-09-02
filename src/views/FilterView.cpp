//
// Created by tomasz on 8/10/24.
//
#include "views/FilterView.h"
#include "imgui.h"

#include "PcapFilter.h"

#include <vector>
#include <string>

#include "imgui_internal.h"

void FilterView::draw(std::shared_ptr<FilterModel> model) {
    ImGui::SetWindowSize("Okno 2", ImVec2(1200.0f, 250.0f));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 200.0f));
    ImGui::Begin("Okno 2", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar );
    ImGui::Columns(4, "mycolumns", false); // 2 kolumny
    ImGui::SetColumnWidth(0,310);
    ImGui::SetColumnWidth(1, 310);
    ImGui::SetColumnWidth(2, 150);
    ImGui::SetColumnWidth(3, 150);
    ImGui::SetNextItemWidth(300);
    this->addressIpSection(model);
    ImGui::NextColumn();
    ImGui::SetNextItemWidth(300);
    this->addressMacSection(model);
    ImGui::NextColumn();
    ImGui::SetNextItemWidth(300);
    this->tcpFlagsSection(model);
    ImGui::NextColumn();
    ImGui::SetNextItemWidth(300);
    this->protocolsSection(model);
    ImGui::Columns(1);
    ImGui::SetCursorPosX((1200 - 400) * 0.5f);
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 50);
    //TODO naprawic pole najezdzanai na przycisk, prawa strona przycisku nie wychwytuje myszki
    if (ImGui::Button("Zatwierdz", ImVec2(400, 50))) {
        model->setUpdate(true);
    }
    ImGui::End();
}

void FilterView::addressIpSection(std::shared_ptr<FilterModel> model) {

    ImGui::InputText("##Label_ip", this->buf_ip, sizeof(this->buf_ip));
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("ADDRESS IP HERE");
    }
    if (ImGui::Button("ADD_IP", ImVec2(300.0f, 20.0f))) {
        const std::string address(this->buf_ip);
        if (!address.empty()) {
            model->addIpFilter(pcpp::IPAddress(address), pcpp::IPFilter(address, pcpp::SRC_OR_DST));
            strcpy(this->buf_ip, "");
        }
    }
    if (ImGui::BeginTable("ip_table", 2, ImGuiTableFlags_Borders
        | ImGuiTableFlags_ScrollX, ImVec2(300, 80)))
    {
        ImGui::TableSetupColumn("IP Address", ImGuiTableColumnFlags_WidthFixed, 180.0f);
        ImGui::TableSetupColumn("Operations_ip", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableHeadersRow();
        for (int row = 0; row < model->getIpFilterVector().size(); row++)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            std::string address{model->getIpFilterVector()[row].first.toString()};
            ImGui::Text("%s",address.c_str());

            ImGui::TableNextColumn();
            if (ImGui::Button("Delete_ip"))
            {
                model->getIpFilterVector().erase(model->getIpFilterVector().begin() + row);
                row--;
            }
        }
        ImGui::EndTable();
    }
}

void FilterView::addressMacSection(std::shared_ptr<FilterModel> model) {

    ImGui::InputText("##Label_mac", this->buf_mac, sizeof(this->buf_mac));
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("ADDRESS MAC HERE");
    }
    if (ImGui::Button("ADD_MAC", ImVec2(300.0f, 20.0f))) {
        const std::string address(this->buf_mac);
        if (!address.empty()) {
            model->addMacFilter(pcpp::MacAddress(address), pcpp::MacAddressFilter(address, pcpp::SRC_OR_DST));
            strcpy(this->buf_mac, "");
        }
    }
    if (ImGui::BeginTable("mac_table", 2, ImGuiTableFlags_Borders
        | ImGuiTableFlags_ScrollX, ImVec2(300, 80)))
    {
        ImGui::TableSetupColumn("MAC Address", ImGuiTableColumnFlags_WidthFixed, 180.0f);
        ImGui::TableSetupColumn("Operations_mac", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableHeadersRow();
        for (int row = 0; row < model->getMacFilterVector().size(); row++)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            std::string address{model->getMacFilterVector()[row].first.toString()};
            ImGui::Text("%s",address.c_str());

            ImGui::TableNextColumn();
            if (ImGui::Button("Delete_mac"))
            {
                model->getMacFilterVector().erase(model->getMacFilterVector().begin() + row);
                row--;
            }
        }
        ImGui::EndTable();
    }
}

void FilterView::tcpFlagsSection(std::shared_ptr<FilterModel> model) {
    ImGui::Text("TCP flags");
    ImGui::BeginChild("InnerColumnsTcpFlags");
    ImGui::Columns(2, "tcpFlagsSectionColumns", false);
    ImGui::SetColumnWidth(0,70);
    ImGui::SetColumnWidth(1, 70);
    auto i = 0;
    for (auto &tcpFlag : model->getTcpFlagsVector()) {
        ImGui::Checkbox(tcpFlag.first.c_str(),  tcpFlag.second.get());
        if (i == 3) {
            ImGui::NextColumn();
        }
        i++;
    }
    ImGui::Columns(1);
    ImGui::EndChild();
}

void FilterView::protocolsSection(std::shared_ptr<FilterModel> model) {
    ImGui::Text("Protocols");
    ImGui::BeginChild("InnerColumnsProtocol");
    ImGui::Columns(2, "protocolSectionColumns", false);
    ImGui::SetColumnWidth(0,70);
    ImGui::SetColumnWidth(1, 70);
    auto i = 0;
    for (auto &ethType : model->getEtherTypeFilterVector()) {
        ImGui::Checkbox(ethType.first.c_str(),  ethType.second.get());
        if (i == 3) {
            ImGui::NextColumn();
        }
        i++;
    }
    ImGui::Columns(1);
    ImGui::EndChild();
}


