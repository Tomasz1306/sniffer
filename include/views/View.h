#ifndef VIEW_H
#define VIEW_H

#include <string>

#include "imgui.h"

class View {
public:
    virtual ~View() = default;

    std::string getTitle() const;

protected:
    bool isWindowInitialized{false};
    bool isWindowOpened{false};
    std::string windowTitle{""};
    int windowFlags{0};
    float windowWidth{0};
    float windowHeight{0};
    float windowX{0};
    float windowY{0};
    ImVec4 darkGreenBorderColor = ImVec4(0.0f, 0.5f, 0.0f, 1.0f); // Ciemny zielony
    ImVec4 darkRedBorderColor = ImVec4(0.5f, 0.0f, 0.0f, 1.0f);   // Ciemny czerwony
    ImVec4 darkBlueBorderColor = ImVec4(0.0f, 0.0f, 0.5f, 1.0f);  // Ciemny niebieski
};

#endif // VIEW_H
