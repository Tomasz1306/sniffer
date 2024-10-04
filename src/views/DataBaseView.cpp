//
// Created by tomasz on 02.10.24.
//

#include "views/DataBaseView.h"

#include "imgui.h"

DataBaseView::DataBaseView() {
    this->windowTitle = "DB";
    this->isWindowOpened = true;
}

void DataBaseView::draw(std::shared_ptr<DataBaseController> controller) {
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);
    if (!controller->isConnectedToDataBase()) {
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("Host", this->host , IM_ARRAYSIZE(host));
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("Port", this->port, IM_ARRAYSIZE(port));
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("User Name", this->username, IM_ARRAYSIZE(username));
        ImGui::SetNextItemWidth(200);
        ImGui::InputText("Password", this->password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);

        if (ImGui::Button("Connect", ImVec2(200, 20))) {
            controller->connectToDataBase(host, port, username, password);
            controller->loadDatabases();
        }
    } else {
        if (ImGui::BeginTable("databaseTable", 1, 0, ImVec2(200.0f,0.0f))) {
            ImGui::TableSetupColumn("Database");
            ImGui::TableHeadersRow();
            for (auto & database : controller->getDatabases()) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text("%s", database.c_str());
            }
            ImGui::EndTable();
        }

        if (ImGui::Button("Refresh", ImVec2(200, 20))) {
            controller->loadDatabases();
        }
        if (ImGui::Button("New database", ImVec2(200, 20))) {
            ImGui::OpenPopup("newDatabasePopup");
        }
        if (ImGui::BeginPopup("newDatabasePopup")) {
            ImGui::InputText("New database", this->newDatabase, IM_ARRAYSIZE(this->newDatabase));
            if (ImGui::Button("Create" , ImVec2(200, 20))) {
                if (this->newDatabase != "") {
                    controller->newDatabase(this->newDatabase);
                    this->newDatabaseWindow = false;
                    memset(this->newDatabase, 0 , IM_ARRAYSIZE(this->newDatabase));
                }
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Disconnect", ImVec2(200, 20))) {
            controller->disconnectFromDataBase();
        }
    }
    ImGui::End();
}

