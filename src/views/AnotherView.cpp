#include "views/AnotherView.h"
#include "imgui.h"

void AnotherView::draw() {
    ImGui::Begin("Another View");
    ImGui::Text("This is another view.");
    ImGui::End();
}
