#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <vector>
#include <memory>

#include "views/View.h"
#include "models/Model.h"

class MainController : Model {
public:
    MainController(std::shared_ptr<Model> model, std::shared_ptr<View> view);
    void run();
    void update();
    void display();
private:
    std::shared_ptr<View> view;
    std::shared_ptr<Model> model;
};

#endif // MAINCONTROLLER_H
