//
// Created by tomasz on 02.10.24.
//

#include "views/LogView.h"
#include "imgui.h"

LogView::LogView() {
    this->windowTitle = "LOGS";
    this->isWindowOpened = true;
}

void LogView::draw() {
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);
    if (ImGui::BeginTable("LogTable__", 3, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        // Ustawienia nagłówków kolumn
        ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed, 150);
        ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthFixed, 450);
        ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 100);
        ImGui::TableHeadersRow();
        for (auto &log : LogController::getInstance()->getLogs()) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", log.date.c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s", log.description.c_str());
            if (ImGui::IsItemHovered()) {
                ImGui::BeginTooltip();
                ImGui::Text("%s", log.description.c_str());
                ImGui::EndTooltip();
            }
            ImGui::TableSetColumnIndex(2);
            switch(log.type) {
                case LogType::ERROR:
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255));
                    ImGui::Text("Error");
                    break;
                case LogType::WARNING:
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
                    ImGui::Text("Warning");
                    break;
                case LogType::SUCCESFULL:
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                    ImGui::Text("Succesfull");
                    break;
            }
            ImGui::PopStyleColor();
        }
        ImGui::EndTable();
    }
    ImGui::End();
}