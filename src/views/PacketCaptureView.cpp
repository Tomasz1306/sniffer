#include "views/PacketCaptureView.h"
#include "imgui.h"

void PacketCaptureView::draw()
{
    ImGui::Begin("Okno", nullptr, ImGuiWindowFlags_NoCollapse);
    ImGui::SetWindowSize("Okno", ImVec2(1300.0f, 800.0f));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 195.0f));

    ImGui::SetWindowFontScale(2.0);
    if (ImGui::BeginTable("Packets", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Index", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("IP SRC", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("IP DST", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("TIME", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("PROTOCOL", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableSetupColumn("TEMP", ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();  // Dodaje nagłówki kolumn

        ImGui::EndTable();
    }

    ImGui::End();


}