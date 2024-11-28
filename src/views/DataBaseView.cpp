//
// Created by tomasz on 02.10.24.
//

#include "views/DataBaseView.h"
#include "controllers/LogController.h"
#include "utils/Utils.h"
#include "global/Global.h"

#include "imgui.h"

DataBaseView::DataBaseView() {
    this->windowTitle = "DB";
    this->isWindowOpened = true;
}

void DataBaseView::draw(std::shared_ptr<DataBaseController> controller) {
    ImGui::Begin(this->windowTitle.c_str(), nullptr, this->windowFlags);
    ImGui::PushFont(Utils::getInstance()->getLargerFont());
    if (!controller->isConnectedToDataBase()) {
        ImGui::SetNextItemWidth(300);
        ImGui::InputText("Host", this->host , IM_ARRAYSIZE(host));
        ImGui::SetNextItemWidth(300);
        ImGui::InputText("Port", this->port, IM_ARRAYSIZE(port));
        ImGui::SetNextItemWidth(300);
        ImGui::InputText("User Name", this->username, IM_ARRAYSIZE(username));
        ImGui::SetNextItemWidth(300);
        ImGui::InputText("Password", this->password, IM_ARRAYSIZE(password), ImGuiInputTextFlags_::ImGuiInputTextFlags_Password);

        if (ImGui::Button("Connect", ImVec2(300, 40))) {
            controller->connectToDataBase(host, port, username, password);
            controller->loadDatabases();
        }
    } else {
        if (ImGui::BeginTable("databaseTable", 1, 0, ImVec2(300.0f,0.0f))) {
            ImGui::TableSetupColumn("Database");
            ImGui::TableHeadersRow();
            int i = 0;
            if (controller->isConnectedToDataBase()) {

                for (auto & database : controller->getDatabases()) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    if (ImGui::Selectable(std::string("##" + database.first).c_str(), database.second)) {
                        if (controller->getSelectedDatabaseIndex() == -1) {
                            database.second = !database.second;
                            controller->selectDatabaseIndex(i);
                            controller->useDatabase();
                            controller->createTables();
                        } else if (controller->getSelectedDatabaseIndex() != -1 && database.second) {
                            database.second = !database.second;
                            controller->selectDatabaseIndex(-1);
                        } else {
                            LogController::getInstance()->addLog(Utils::getTime(), "CANT OPEN DATABASE BECAUSE OTHER DATABASE IS CURRENTLY SELECTED", LogType::WARNING);
                        }
                    }

                    if (database.second) {

                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 255, 0, 255));
                    } else {
                        ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 255, 255, 255));
                    }
                    ImGui::SameLine();
                    ImGui::Text("%s", database.first.c_str());
                    ImGui::PopStyleColor();
                    i++;
                }
            }
            ImGui::EndTable();
        }

        // if (ImGui::Button("Refresh", ImVec2(300, 40))) {
        //     controller->loadDatabases();
        // }
        if (ImGui::Button("New database", ImVec2(300, 40))) {
            ImGui::OpenPopup("newDatabasePopup");
        }
        if (ImGui::BeginPopup("newDatabasePopup")) {
            ImGui::InputText("New database", this->newDatabase, IM_ARRAYSIZE(this->newDatabase));
            if (ImGui::Button("Create" , ImVec2(300, 40))) {
                if (this->newDatabase != "") {
                    controller->newDatabase(this->newDatabase);
                    this->newDatabaseWindow = false;
                    memset(this->newDatabase, 0 , IM_ARRAYSIZE(this->newDatabase));
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Disconnect", ImVec2(300, 40))) {
            controller->disconnectFromDataBase();
        }
        this->drawProgressBar(controller);
    }

    ImGui::PopFont();
    ImGui::End();
}

void DataBaseView::drawProgressBar(std::shared_ptr<DataBaseController> controller) {
    float progress = 1.0f - (controller->getCapturedPacketVectorDatabaseSize() / controller->getMaxVectorSize());
    ImGui::Text("Number of packets in vector: %d", controller->getCapturedPacketVectorDatabaseSize());
    ImGui::Text("Number of threads: %d", controller->getNumberOfThreads());
    ImGui::ProgressBar(progress);
}


