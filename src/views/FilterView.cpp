//
// Created by tomasz on 8/10/24.
//
#include "views/FilterView.h"
#include "imgui.h"

#include "PcapFilter.h"

#include <vector>
#include <string>

void FilterView::draw(std::shared_ptr<FilterModel> model) {
    ImGui::SetWindowSize("Okno 2", ImVec2(1200.0f, 250.0f));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 270.0f));
    ImGui::Begin("Okno 2", nullptr,
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar );
    ImGui::SetNextItemWidth(300);
    this->addressIpSection(model);



    ImGui::End();
}

void FilterView::addressIpSection(std::shared_ptr<FilterModel> model) {

    ImGui::InputText("##Label_ip", this->buf, sizeof(this->buf));
    if (ImGui::IsItemHovered()) {
        ImGui::SetTooltip("ADDRESS IP HERE");
    }
    if (ImGui::Button("ADD", ImVec2(300.0f, 20.0f))) {
        const std::string address(this->buf);
        if (!address.empty()) {
            model->addIpFilter(pcpp::IPAddress(address), pcpp::IPFilter(address, pcpp::SRC_OR_DST));
            strcpy(this->buf, "");
        }
    }
    if (ImGui::BeginTable("table_with_button", 2, ImGuiTableFlags_Borders
        | ImGuiTableFlags_ScrollX, ImVec2(300, 80)))
    {
        ImGui::TableSetupColumn("IP Address", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableSetupColumn("Operations", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableHeadersRow();
        for (int row = 0; row < model->getIpFilterVector().size(); row++)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            std::string address{model->getIpFilterVector()[row].first.toString()};
            ImGui::Text("%s",address.c_str());

            ImGui::TableNextColumn();
            if (ImGui::Button("Delete"))
            {
                model->getIpFilterVector().erase(model->getIpFilterVector().begin() + row);
                row--;
            }
        }
        ImGui::EndTable();
    }
}
