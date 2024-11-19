//
// Created by tomasz on 11/12/24.
//

#include "views/SearchView.h"

SearchView::SearchView() {
    this->windowTitle = "SEARCH";
    this->windowHeight = 1000.0f;
    this->windowWidth = 800.0f;
    this->isWindowOpened = false;
}

void SearchView::draw(std::shared_ptr<SearchController> controller) {
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);
    ImGui::InputText("Search Text", this->searchText, 300);

    if (ImGui::Button("Search")) {
        controller->search(std::string(this->searchText));
    }
    if (controller->getFindedPackets().size() > 0) {
        if (ImGui::BeginTable("FindedPacketsTable", 1, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
            // Ustawienia nagłówków kolumn
            ImGui::TableSetupColumn("Finded Packets", ImGuiTableColumnFlags_WidthFixed, 300);
            ImGui::TableHeadersRow();
            for (auto &packet : controller->getFindedPackets()) {
                ImGui::TableSetColumnIndex(0);
                std::string label = std::to_string(packet.id) + "  " + packet.captureTime;
                if (ImGui::Selectable(label.c_str())) {
                    controller->displayPacket(packet.id);
                }
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();
}

