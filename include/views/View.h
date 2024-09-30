#ifndef VIEW_H
#define VIEW_H

#include <string>

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
};

#endif // VIEW_H
