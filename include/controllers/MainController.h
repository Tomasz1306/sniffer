#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <vector>
#include <memory>

#include "views/View.h"
#include "views/PacketCaptureView.h"
#include "models/Model.h"
#include "models/PacketCaptureModel.h"

class MainController : Model {
public:
    MainController(std::shared_ptr<PacketCaptureModel> model, std::shared_ptr<PacketCaptureView> view);
    void run();
    void update();
    void display();
private:
    std::shared_ptr<PacketCaptureView> view;
    std::shared_ptr<PacketCaptureModel> model;
};

#endif // MAINCONTROLLER_H
