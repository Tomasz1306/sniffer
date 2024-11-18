#include "controllers/MainController.h"
#include "views/PacketCaptureView.h"
#include "models/PacketCaptureModel.h"
#include "listeners/Listener.h"
#include "utils/Utils.h"
#include "controllers/FilterController.h"
#include "models/FilterModel.h"
#include "views/FilterView.h"
#include "controllers/DeviceController.h"
#include "models/DeviceModel.h"
#include <views/DeviceView.h>
#include <views/PacketView.h>
#include "controllers/StatisticController.h"
#include "models/StatisticModel.h"
#include "views/StatisticView.h"
#include "controllers/LogController.h"
#include "models/LogModel.h"
#include "views/LogView.h"
#include "controllers/DataBaseController.h"
#include "models/DataBaseModel.h"
#include "views/DataBaseView.h"
#include "controllers/AnalyzerController.h"
#include "models/AnalyzerModel.h"
#include "views/AnalyzerView.h"
#include "controllers/SearchController.h"
#include "models/SearchModel.h"
#include "views/SearchView.h"

#include "imgui.h"
#include "implot.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <Packet.h>
#include <PcapFileDevice.h>
#include <PcapLiveDevice.h>
#include <PcapLiveDeviceList.h>
#include <IPv4Layer.h>

#include <iostream>
#include <vector>
#include <ctime>

//TODO WYSWIETLAC PAKIETY ZE ZLA SUMA KONTROLNA I DODAC DO STATYSTYK
//TODO zapewnic przesuwanie po osi x
//TODO ZAPISYWANIE POJEDYNCZEGO PAKIETU DO PLIKU
//TODO PRZYCISKI DO PRZECHODZENIA NA GORE I NA DOL LISTY
//TODO FILTROWANIE WYSWIETLANIA
void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
    auto utils = Utils::getInstance();
    // Ustawienie callbacka błędów GLFW
    glfwSetErrorCallback(glfw_error_callback);

    // Inicjalizacja GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    last_time = std::chrono::steady_clock::now();

    // Tworzenie okna z kontekstem OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(1800, 1000, "SNIFFER", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Włącz V-sync

    // Ustawienia kontekstu ImGui
    // IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    Utils::getInstance()->setSmallerFont(io.Fonts->AddFontFromFileTTF("../rsc/fonts/JetBrainsMonoNL-Regular.ttf", 16.0f));
    Utils::getInstance()->setLargerFont(io.Fonts->AddFontFromFileTTF("../rsc/fonts/JetBrainsMonoNL-Regular.ttf", 24.0f));

    // Ustawienia stylu ImGui
    ImGui::StyleColorsDark();

    ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);
    auto& style = ImGui::GetStyle();
    style = ImGuiStyle();
    style.WindowBorderSize = 1.f * 1.0f;
    style.FrameBorderSize = 1.f * 1.0f;
    style.FrameRounding = 5.f;
    // style.Colors[ImGuiCol_ScrollbarBg] = ImVec4( 1, 1, 1, 0.03f );
    // style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    // style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    // style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.45f);
    // style.Colors[ImGuiCol_TitleBgCollapsed] = style.Colors[ImGuiCol_TitleBg];
  style.Colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
style.Colors[ImGuiCol_TextDisabled]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
style.Colors[ImGuiCol_WindowBg]               = ImVec4(0.09f, 0.09f, 0.15f, 1.00f);
style.Colors[ImGuiCol_ChildBg]                = ImVec4(0.11f, 0.11f, 0.20f, 1.00f);
style.Colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.16f, 1.00f);
style.Colors[ImGuiCol_Border]                 = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
style.Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
style.Colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.20f, 0.23f, 1.00f);
style.Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.35f, 0.40f, 0.70f, 0.69f); // Changed color
style.Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.47f, 0.47f, 0.73f, 0.68f); // Changed color
style.Colors[ImGuiCol_TitleBg]                = ImVec4(0.15f, 0.15f, 0.25f, 1.00f);
style.Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.20f, 0.20f, 0.30f, 1.00f);
style.Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.10f, 0.10f, 0.15f, 1.00f);
style.Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.23f, 1.00f);
style.Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.10f, 1.00f);
style.Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.38f, 1.00f);
style.Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.67f, 0.67f, 0.70f, 1.00f); // Changed color
style.Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.74f, 0.74f, 0.78f, 1.00f); // Changed color
style.Colors[ImGuiCol_CheckMark]              = ImVec4(0.38f, 0.72f, 0.38f, 1.00f);
style.Colors[ImGuiCol_SliderGrab]             = ImVec4(0.48f, 0.48f, 0.52f, 1.00f);
style.Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.74f, 0.74f, 0.78f, 1.00f); // Changed color
style.Colors[ImGuiCol_Button]                 = ImVec4(0.24f, 0.24f, 0.28f, 1.00f);
style.Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.35f, 0.40f, 0.70f, 0.69f); // Changed color
style.Colors[ImGuiCol_ButtonActive]           = ImVec4(0.47f, 0.47f, 0.73f, 0.68f); // Changed color
style.Colors[ImGuiCol_Header]                 = ImVec4(0.24f, 0.24f, 0.28f, 1.00f);
style.Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.35f, 0.40f, 0.70f, 0.69f); // Changed color
style.Colors[ImGuiCol_HeaderActive]           = ImVec4(0.47f, 0.47f, 0.73f, 0.68f); // Changed color
style.Colors[ImGuiCol_Separator]              = ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
style.Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.75f, 0.75f, 0.78f, 1.00f); // Changed color
style.Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.85f, 0.85f, 0.88f, 1.00f); // Changed color
style.Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.44f, 0.44f, 0.48f, 1.00f);
style.Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.75f, 0.75f, 0.78f, 1.00f); // Changed color
style.Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.85f, 0.85f, 0.88f, 1.00f); // Changed color
style.Colors[ImGuiCol_Tab]                    = ImVec4(0.21f, 0.21f, 0.26f, 1.00f);
style.Colors[ImGuiCol_TabHovered]             = ImVec4(0.35f, 0.40f, 0.70f, 0.80f); // Changed color
style.Colors[ImGuiCol_TabActive]              = ImVec4(0.47f, 0.47f, 0.73f, 0.85f); // Changed color
style.Colors[ImGuiCol_TabUnfocused]           = ImVec4(0.21f, 0.21f, 0.26f, 1.00f);
style.Colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.35f, 0.40f, 0.70f, 0.75f); // Changed color
style.Colors[ImGuiCol_DockingPreview]         = ImVec4(0.88f, 0.88f, 0.88f, 0.70f);
style.Colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
style.Colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
style.Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
style.Colors[ImGuiCol_PlotHistogram]          = ImVec4(0.73f, 0.43f, 0.27f, 1.00f);
style.Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.30f, 1.00f);
style.Colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.27f, 0.27f, 0.32f, 1.00f);
style.Colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.37f, 0.37f, 0.42f, 1.00f);
style.Colors[ImGuiCol_TableBorderLight]       = ImVec4(0.30f, 0.30f, 0.34f, 1.00f);
style.Colors[ImGuiCol_TableRowBg]             = ImVec4(0.19f, 0.19f, 0.22f, 1.00f);
style.Colors[ImGuiCol_TableRowBgAlt]          = ImVec4(0.23f, 0.23f, 0.26f, 1.00f);
style.Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.37f, 0.63f, 0.87f, 0.87f);
style.Colors[ImGuiCol_DragDropTarget]         = ImVec4(0.97f, 0.39f, 0.31f, 1.00f);
style.Colors[ImGuiCol_NavHighlight]           = ImVec4(0.35f, 0.35f, 0.40f, 1.00f);
style.Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(0.65f, 0.65f, 0.70f, 0.70f);
style.Colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.20f, 0.20f, 0.25f, 0.70f);
style.Colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.20f, 0.20f, 0.25f, 0.70f);
    // Inicjalizacja ImGui dla GLFW i OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    auto searchView = std::make_shared<SearchView>();
    auto searchModel = std::make_shared<SearchModel>();
    auto analyzerView = std::make_shared<AnalyzerView>();
    auto analyzerModel = std::make_shared<AnalyzerModel>();
    auto dataBaseView = std::make_shared<DataBaseView>();
    auto dataBaseModel = std::make_shared<DataBaseModel>();
    auto logView = std::make_shared<LogView>();
    auto logModel = std::make_shared<LogModel>(Utils::getTime());
    auto statisticView = std::make_shared<StatisticView>();
    auto statisticModel = std::make_shared<StatisticModel>(Utils::getTime());
    auto packetView = std::make_shared<PacketView>();
    auto packetCaptureView = std::make_shared<PacketCaptureView>();
    auto packetCaptureModel = std::make_shared<PacketCaptureModel>();
    auto filterView = std::make_shared<FilterView>();
    auto filterModel = std::make_shared<FilterModel>();
    auto deviceView = std::make_shared<DeviceView>();
    auto deviceModel = std::make_shared<DeviceModel>();
    auto packetListener = std::make_shared<Listener>(packetCaptureModel);
    LogController::getInstance();
    LogController::getInstance()->setModel(logModel);
    LogController::getInstance()->setView(logView);
    auto searchController = std::make_shared<SearchController>(searchModel, searchView);
    auto analyzerController = std::make_shared<AnalyzerController>(analyzerModel, analyzerView);
    auto dataBaseController = std::make_shared<DataBaseController>(dataBaseModel, dataBaseView);
    auto statisticController = std::make_shared<StatisticController>(statisticModel, statisticView, packetListener);
    auto filterController = std::make_shared<FilterController>(filterModel, filterView, packetListener);
    auto deviceController = std::make_shared<DeviceController>(deviceModel, deviceView, packetListener);
    auto mainController = std::make_shared<MainController>(packetCaptureModel,
                                                            packetCaptureView,
                                                            packetListener,
                                                            packetView,
                                                            analyzerController,
                                                            dataBaseController,
                                                            filterController,
                                                            statisticController,
                                                            deviceController,
                                                            searchController);
    dataBaseController->setMainController(mainController);
    packetCaptureModel->setMainController(mainController);
    filterController->setMainController(mainController);
    analyzerController->setMainController(mainController);
    deviceController->setMainController(mainController);
    statisticController->setMainController(mainController);
    searchController->setMainController(mainController);
    bool dockInitialized = false;

    while (!glfwWindowShouldClose(window)) {

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (!dockInitialized) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);
            ImGui::DockBuilderSetNodeSize(dockspace_id, ImGui::GetMainViewport()->Size);

            ImGuiID dock_main_id = dockspace_id;
            ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id,
                ImGuiDir_Left, 0.6f, nullptr, &dock_main_id);
            ImGuiID dock_id_right = dock_main_id;

            ImGuiID dock_id_right_top = ImGui::DockBuilderSplitNode(dock_id_right,
                ImGuiDir_Up, 0.4f, nullptr, &dock_id_right);

            ImGuiID dock_left_top = ImGui::DockBuilderSplitNode(dock_id_left,
                ImGuiDir_Up, 0.15f, nullptr, &dock_id_left);
            ImGuiID dock_left_mid = ImGui::DockBuilderSplitNode(dock_id_left,
                ImGuiDir_Up, 0.25f, nullptr, &dock_id_left);
            ImGuiID dock_left_bot = dock_id_left;

            ImGuiID dock_left_top_left = ImGui::DockBuilderSplitNode(dock_left_top,
                ImGuiDir_Left, 0.5f, nullptr, &dock_left_top);
            ImGuiID dock_left_top_right = dock_left_top;
            ImGui::DockBuilderDockWindow("DEVICES", dock_left_top_left);
            ImGui::DockBuilderDockWindow("FILTERS", dock_left_mid);
            ImGui::DockBuilderDockWindow("PACKETS", dock_left_bot);
            ImGui::DockBuilderDockWindow("STATISTICS", dock_id_right_top);
            ImGui::DockBuilderDockWindow("ANALYZER", dock_left_top_right);
            ImGui::DockBuilderDockWindow("SEARCH", dock_left_top_right);
            ImGui::DockBuilderDockWindow("LOGS", dock_id_right);
            ImGui::DockBuilderDockWindow("DB", dock_id_right);
            ImGui::DockBuilderFinish(dockspace_id);
            dockInitialized = true;
        }

        mainController->display();
        filterController->display();
        deviceController->display();
        statisticController->display();
        dataBaseController->display();
        LogController::getInstance()->display();
        analyzerController->display();
        searchController->display();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    packetListener->stopCapturePackets();
    packetListener->closeListener();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
