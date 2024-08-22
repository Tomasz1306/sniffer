#include "controllers/MainController.h"
#include "views/PacketCaptureView.h"
#include "views/PacketCaptureView.h"

MainController::MainController(std::shared_ptr<PacketCaptureModel> _model, std::shared_ptr<PacketCaptureView> _view, std::shared_ptr<Listener> _listener) {
    this->model = _model;
    this->view = _view;
    this->listener = _listener;
}

void MainController::run() {

}

void MainController::update(){

}

void MainController::display(){
    this->view->draw(this->getSharedPtr(), this->model->getCapturedPacketVector());
}

void MainController::startCapture() {
    this->listener->startCapturePackets();
}

void MainController::stopCapture() {
    this->listener->stopCapturePackets();
}

void MainController::clearTableOfPackets() {

}
