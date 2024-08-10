#include "controllers/MainController.h"
#include "views/PacketCaptureView.h"
#include "views/PacketCaptureView.h"

MainController::MainController(std::shared_ptr<PacketCaptureModel> model, std::shared_ptr<PacketCaptureView> view) {
    this->model = model;
    this->view = view;
}

void MainController::run() {

}

void MainController::update(){

}

void MainController::display(){
    this->view->draw(this->model->getCapturedPacketVector());
}
