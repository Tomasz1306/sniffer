#include "controllers/MainController.h"
#include "views/MainView.h"
#include "views/AnotherView.h"
#include "views/PacketCaptureView.h"
#include "services/RenderService.h"

MainController::MainController(std::shared_ptr<Model> model, std::shared_ptr<View> view) {
    this->model = model;
    this->view = view;
}

void MainController::run() {

}

void MainController::update(){

}

void MainController::display(){

}