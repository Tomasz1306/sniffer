//
// Created by tomasz on 8/10/24.
//
#include "views/FilterView.h"
#include "imgui.h"

#include <vector>
#include <string>

void FilterView::draw() {
    ImGui::SetWindowSize("Okno 2", ImVec2(1800.0f, 200.0f));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::Begin("Okno 2", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

    ImGui::SetNextItemWidth(300);
    ImGui::InputText("Wprowadz address IP", this->buf, sizeof(this->buf));

    if (ImGui::Button("Dodaj Adress IP")) {
        const std::string address(this->buf);
        if (!address.empty()) {
            this->items.push_back(address);
            strcpy(this->buf, "");
        }
    }
    if (ImGui::BeginTable("table_with_button", 2, ImGuiTableFlags_Borders
        | ImGuiTableFlags_ScrollX, ImVec2(300, 80)))
    {
        ImGui::TableSetupColumn("IP Address", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableSetupColumn("Operations", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableHeadersRow();
        for (int row = 0; row < this->items.size(); row++)
        {
            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text(this->items[row].c_str());

            ImGui::TableNextColumn();
            if (ImGui::Button("Delete"))
            {
                this->items.erase(this->items.begin() + row);
                row--;
            }
        }
        ImGui::EndTable();
    }
    ImGui::Text("WItam");

    ImGui::End();
}