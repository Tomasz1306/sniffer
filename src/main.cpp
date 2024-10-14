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
#include "controllers/WindowManagerController.h"
#include "models/WindowManagerModel.h"
#include "views/WindowManagerView.h"
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

//TODO uporzadkowac includy i usunca nieporzebne
//TODO Napisac "forward declaration" dla kazdego zbioru MVC
//TODO zabezpieczyc wybieranie innego interfacu jezeli inny jest otwarty
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
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4( 1, 1, 1, 0.03f );
    style.Colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.45f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = style.Colors[ImGuiCol_TitleBg];
    // Inicjalizacja ImGui dla GLFW i OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    auto analyzerView = std::make_shared<AnalyzerView>();
    auto analyzerModel = std::make_shared<AnalyzerModel>();
    auto dataBaseView = std::make_shared<DataBaseView>();
    auto dataBaseModel = std::make_shared<DataBaseModel>();
    auto logView = std::make_shared<LogView>();
    auto logModel = std::make_shared<LogModel>(Utils::getTime());
    auto statisticView = std::make_shared<StatisticView>();
    auto statisticModel = std::make_shared<StatisticModel>(Utils::getTime());
    auto windowManagerView = std::make_shared<WindowManagerView>();
    auto windowManagerModel = std::make_shared<WindowManagerModel>();
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
    auto analyzerController = std::make_shared<AnalyzerController>(analyzerModel, analyzerView);
    auto dataBaseController = std::make_shared<DataBaseController>(dataBaseModel, dataBaseView);
    auto statisticController = std::make_shared<StatisticController>(statisticModel, statisticView, packetListener);
    auto windowManagerController = std::make_shared<WindowManagerController>(windowManagerModel, windowManagerView);
    windowManagerController->addView(windowManagerView);
    windowManagerController->addView(packetView);
    windowManagerController->addView(packetCaptureView);
    windowManagerController->addView(filterView);
    windowManagerController->addView(deviceView);
    auto filterController = std::make_shared<FilterController>(filterModel, filterView, packetListener);
    auto deviceController = std::make_shared<DeviceController>(deviceModel, deviceView, packetListener);
    auto mainController = std::make_shared<MainController>(packetCaptureModel,
                                                            packetCaptureView,
                                                            packetListener,
                                                            packetView,
                                                            statisticController,
                                                            deviceController);
    dataBaseController->setMainController(mainController);
    packetCaptureModel->setMainController(mainController);
    filterController->setMainController(mainController);
    analyzerController->setMainController(mainController);
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
            //ImGui::DockBuilderDockWindow("WINDOW MANAGER", dock_left_top_right);
            ImGui::DockBuilderDockWindow("FILTERS", dock_left_mid);
            ImGui::DockBuilderDockWindow("PACKETS", dock_left_bot);
            ImGui::DockBuilderDockWindow("STATISTICS", dock_id_right_top);
            ImGui::DockBuilderDockWindow("ANALYZER", dock_left_top_right);

            ImGui::DockBuilderDockWindow("LOGS", dock_id_right);
            ImGui::DockBuilderDockWindow("DB", dock_id_right);
            ImGui::DockBuilderFinish(dockspace_id);
            dockInitialized = true;
        }

        mainController->display();
        filterController->display();
        deviceController->display();
        //windowManagerController->display();
        statisticController->display();
        dataBaseController->display();
        LogController::getInstance()->display();

        analyzerController->display();

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
