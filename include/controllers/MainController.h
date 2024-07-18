#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <vector>
#include <memory>

#include "views/View.h"

class MainController {
public:
    MainController();
    void run();
private:
    std::vector<std::shared_ptr<View>> views;
    void setupViews();
};

#endif // MAINCONTROLLER_H
