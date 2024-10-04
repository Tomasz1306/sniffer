//
// Created by tomasz on 02.10.24.
//

#include "views/AnalyzerView.h"

#include "imgui.h"

AnalyzerView::AnalyzerView() {
    this->windowTitle = "ANALYZER";
    this->isWindowOpened = true;
}

void AnalyzerView::draw(std::shared_ptr<AnalyzerController> controller) {
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);

    ImGui::End();
}