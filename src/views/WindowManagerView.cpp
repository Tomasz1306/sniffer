//
// Created by tomasz on 23.09.24.
//

#include "views/WindowManagerView.h"
#include "imgui.h"

WindowManagerView::WindowManagerView() {
    this->windowTitle = "WINDOW MANAGER";
    this->windowHeight = 600.0f;
    this->windowWidth = 200.0f;
    this->windowX = 600.0f;
    this->windowY = 0.0f;
    this->isWindowOpened = true;
    this->windowFlags = 0;
}

void WindowManagerView::draw(std::shared_ptr<WindowManagerController> controller) {
    ImGui::SetWindowSize(this->windowTitle.c_str(), ImVec2(this->windowHeight, this->windowWidth));
    if (!this->isWindowInitialized) {
        ImGui::SetNextWindowPos(ImVec2(this->windowX, this->windowY));
        this->isWindowInitialized = true;
    }
    ImGui::Begin(this->windowTitle.c_str(), NULL, this->windowFlags);

    for (auto &view : controller->getViews()) {
        ImGui::SameLine();
        if (ImGui::Button(std::string(view->getTitle() + "_").c_str(), ImVec2(100.0f, 50.0f))) {

        }
    }

    ImGui::End();
}
