//
// Created by tomasz on 02.10.24.
//

#include "views/AnalyzerView.h"
#include "global/Global.h"
#include "imgui.h"

AnalyzerView::AnalyzerView()
{
    this->windowTitle = "ANALYZER";
    this->isWindowOpened = true;
}

void AnalyzerView::draw(std::shared_ptr<AnalyzerController> controller)
{
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);

    if (!controller->getIsAnalyzingEnable()) {
        if (ImGui::Button("Enable analyzer", ImVec2(200, 20))) {
            controller->setIsAnalyzingEnable(true);
        }
    } else {
        if (ImGui::Button("Disable analyzer", ImVec2(200, 20))) {
            controller->setIsAnalyzingEnable(false);
        }
        std::chrono::steady_clock::time_point current_time = std::chrono::steady_clock::now();
        std::chrono::seconds::rep elapsed = std::chrono::duration_cast<std::chrono::seconds>(current_time - last_time).count();
        if (elapsed >= 2) {
            std::vector<CapturedPackets> vectorToAnalyze{controller->getCapturedVectorData()};
            controller->getCapturedVectorData().clear();
            controller->analyzePackets(vectorToAnalyze);
            last_time = current_time;
        }
    }

    if (ImGui::BeginTable("WarningTable", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
    {
        // Ustawienia nagłówków kolumn
        ImGui::TableSetupColumn("WARNINGS", ImGuiTableColumnFlags_WidthFixed, 600);
        ImGui::TableHeadersRow();
        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
        for (auto &warning : controller->getWarnings())
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);

            std::lock_guard<std::mutex> lock(warningGuard);
            ImGui::Text("%s", warning.first.c_str());
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("%s : %s", warning.second.ip.c_str(), warning.second.description.c_str());
                ImGui::EndTooltip();
            }
        }
        ImGui::EndTable();
    }
    ImGui::PopStyleColor();

    ImGui::End();
}