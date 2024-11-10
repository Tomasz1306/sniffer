//
// Created by tomasz on 02.10.24.
//

#include "views/AnalyzerView.h"
#include "global/Global.h"
#include "imgui.h"

AnalyzerView::AnalyzerView() {
    this->windowTitle = "ANALYZER";
    this->isWindowOpened = true;
}

void AnalyzerView::draw(std::shared_ptr<AnalyzerController> controller) {
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);
    if (controller->getCapturedVectorData().empty()) {
        ImGui::Text("No packets received");
    } else {
        if (!controller->getIsAnalyzingEnable()) {
            if (ImGui::Button("Enable analyzer", ImVec2(200, 20))) {
                controller->setIsAnalyzingEnable(true);
                controller->analyzePackets(controller->getCapturedVectorData());
            }
        } else {
            analyzerGuard.lock();
            if (controller->getPacketsToAnalyzeCount() != 0 && controller->getAnalyzedPacketsCount() != 0) {
                float progress = 1.0f - (controller->getPacketsToAnalyzeCount() / controller->getAnalyzedPacketsCount());
                ImGui::ProgressBar(progress);
            }
            analyzerGuard.unlock();
        }

        if (ImGui::BeginTable("WarningTable", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            // Ustawienia nagłówków kolumn
            ImGui::TableSetupColumn("WARNINGS", ImGuiTableColumnFlags_WidthFixed, 600);
            ImGui::TableHeadersRow();
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 0, 255));
            for (auto &warning : controller->getWarnings()) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);

                std::lock_guard<std::mutex> lock(warningGuard);
                ImGui::Text("%s", warning.second.c_str());
                if(ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text("%s : %s", warning.first.c_str(), warning.second.c_str());
                    ImGui::EndTooltip();
                }
            }
            ImGui::EndTable();
        }
        ImGui::PopStyleColor();
    }
    ImGui::End();
}