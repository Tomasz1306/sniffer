#ifndef UTILS_H
#define UTILS_H

#include <PcapLiveDevice.h>
#include <string>
#include <ctime>
#include "imgui.h"

class Utils{

public:
    static Utils *getInstance();

    static void onPacketArrivesBlockingMode(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie);
    static std::string formatTimestamp(const std::time_t *tp);
    static std::string getTime();

    void setSmallerFont(ImFont *font);
    void setLargerFont(ImFont *font);
    ImFont *getSmallerFont();
    ImFont *getLargerFont();


    void SetGreenBorderColor() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Border] = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    }


    void SetRedBorderColor() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    }


    void SetWhiteBorderColor() {
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_Border] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
private:
    static Utils *instance;
    Utils(){};

    ImFont *smallerFont;
    ImFont *largerFont;

};


#endif

