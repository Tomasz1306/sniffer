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


#include "imgui.h"
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

//TODO zaimplementowac window menager
//TODO uporzadkowac includy i usunca nieporzebne
//TODO Napisac "forward declaration" dla kazdego zbioru MVC

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

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "SNIFFER", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Włącz V-sync

    // Ustawienia kontekstu ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.Fonts->AddFontFromFileTTF("../rsc/fonts/JetBrainsMonoNL-Regular.ttf", 16.0f);


    // Ustawienia stylu ImGui
    ImGui::StyleColorsDark();
    ImGui::GetStyle().WindowTitleAlign = ImVec2(0.5f, 0.5f);

    // Inicjalizacja ImGui dla GLFW i OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    auto packetView = std::make_shared<PacketView>();
    auto packetCaptureView = std::make_shared<PacketCaptureView>();
    auto packetCaptureModel = std::make_shared<PacketCaptureModel>();
    auto filterView = std::make_shared<FilterView>();
    auto filterModel = std::make_shared<FilterModel>();
    auto deviceView = std::make_shared<DeviceView>();
    auto deviceModel = std::make_shared<DeviceModel>();
    auto packetListener = std::make_shared<Listener>(packetCaptureModel);
    //packetListener->openListener();
    auto filterController = std::make_shared<FilterController>(filterModel, filterView, packetListener);
    auto mainController = std::make_shared<MainController>(packetCaptureModel, packetCaptureView, packetListener, packetView);
    auto deviceController = std::make_shared<DeviceController>(deviceModel, deviceView, packetListener);

    // Główna pętla aplikacji
    while (!glfwWindowShouldClose(window)) {
        // Początek nowej klatki ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Renderowanie widoków
        mainController->display();
        filterController->display();
        deviceController->display();
        // Renderowanie ImGui
        ImGui::Render();
        int display_w, display_h;
        // glfwGetFramebufferSize(window, &display_w, &display_h);
        // glViewport(0, 0, display_w, display_h);
        glClearColor(0, 0, 0, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    packetListener->stopCapturePackets();
    packetListener->closeListener();
    // Clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
