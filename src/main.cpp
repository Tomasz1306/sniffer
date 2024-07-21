#include "controllers/MainController.h"
#include "views/PacketCaptureView.h"
#include "models/PacketCaptureModel.h"

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

void glfw_error_callback(int error, const char* description) {
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

int main() {
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

    GLFWwindow* window = glfwCreateWindow(1500, 1000, "SNIFFER", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0); // Włącz V-sync
    //192.168.0.31
    pcpp::PcapLiveDevice *dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp("127.0.0.1");
    if (dev == NULL)
    {
        std::cerr << "ERROR DEVICE " << std::endl;
    }

    if (!dev->open()) {
        std::cerr << "ERROR DEVICE " << std::endl;
    }
    pcpp::RawPacketVector vec;
    if (dev->startCapture(vec)) {

    }

    

    
    // Ustawienia kontekstu ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    // Ustawienia stylu ImGui
    ImGui::StyleColorsDark();

    // Inicjalizacja ImGui dla GLFW i OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // Tworzenie widokow 
    auto packetCaptureView = std::make_shared<PacketCaptureView>();
    auto packetCaptureModel = std::make_shared<PacketCaptureModel>();
    // Tworzenie głównego kontrolera aplikacji
    // MainController controller_PacketWindow();

    // Główna pętla aplikacji
    while (!glfwWindowShouldClose(window)) {
        // Początek nowej klatki ImGui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Renderowanie widoków
 // Przetwarzaj przechwycone pakiety
    for (pcpp::RawPacketVector::VectorIterator iter = vec.begin(); iter != vec.end(); iter++)
    {
        // Tworzymy obiekt Packet z RawPacket
        pcpp::Packet parsedPacket(*iter);

        // Wyświetl informacje o pakiecie
        std::cout << "Packet captured at: " << (*iter)->getPacketTimeStamp().tv_sec << "." << (*iter)->getPacketTimeStamp().tv_sec << std::endl;
        std::cout << "Packet length: " << (*iter)->getRawDataLen() << " bytes" << std::endl;

        // Sprawdzamy, czy pakiet zawiera warstwę IP
        if (parsedPacket.isPacketOfType(pcpp::IPv4))
        {
            pcpp::IPv4Layer* ipLayer = parsedPacket.getLayerOfType<pcpp::IPv4Layer>();
            if (ipLayer != nullptr)
            {
                std::cout << "Source IP: " << ipLayer->getSrcIPAddress().toString() << std::endl;
                std::cout << "Destination IP: " << ipLayer->getDstIPAddress().toString() << std::endl;
            }
        }

        std::cout << std::endl;
    }
        // Renderowanie ImGui
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    dev->stopCapture();
    dev->close();
    // Clean up
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
