#include "views/MainView.h"
#include "imgui.h"

void MainView::draw() {
    ImGui::Begin("Main View");
    ImGui::Text("This is the main view.");
    ImGui::End();
}
