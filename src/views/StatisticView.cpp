//
// Created by tomasz on 01.10.24.
//

#include "views/StatisticView.h"
#include "imgui.h"
#include "imgui_internal.h"

StatisticView::StatisticView() {
    this->windowTitle = "STATISTICS";
    // this->windowHeight = 600.0f;
    // this->windowWidth = 200.0f;
    // this->windowX = 1200.0f;
    // this->windowY = 0.0f;
    this->isWindowOpened = true;
    // this->windowFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void StatisticView::draw(std::shared_ptr<StatisticController> controller) {
    // ImGui::SetWindowSize(this->windowTitle.c_str(), ImVec2(this->windowHeight, this->windowWidth));
    // if (!this->isWindowInitialized) {
    //     ImGui::SetNextWindowPos(ImVec2(this->windowX, this->windowY));
    //     this->isWindowInitialized = true;
    // }

    ImGui::Begin(this->windowTitle.c_str(), &this->isWindowOpened, this->windowFlags);
    ImGui::CloseButton(20, ImVec2(50.0f, 50.0f));

    ImGui::End();
}