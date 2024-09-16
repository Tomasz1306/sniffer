#include "controllers/MainController.h"
#include "views/PacketCaptureView.h"
#include "views/PacketCaptureView.h"

MainController::MainController(std::shared_ptr<PacketCaptureModel> _model,
    std::shared_ptr<PacketCaptureView> _view,
    std::shared_ptr<Listener> _listener,
    std::shared_ptr<PacketView> _packetView) {
    this->model = _model;
    this->view = _view;
    this->listener = _listener;
    this->packetView = _packetView;
}

void MainController::run() {

}

void MainController::update(){

}

void MainController::display(){
    this->view->draw(this->getSharedPtr(), this->model->getCapturedPacketVector());
    if (this->isPacketDisplayed) {
        this->packetView->draw(this->getSharedPtr(), this->model->getCapturedPacketVector()[this->view->getCurrentSelectedPacketId()]);
    }
}

void MainController::startCapture() {
    this->listener->startCapturePackets();
}

void MainController::stopCapture() {
    this->listener->stopCapturePackets();
}

void MainController::clearTableOfPackets() {

}

void MainController::setIsDisplayedPakcet(bool _isDisplayedPakcet) {
    this->isPacketDisplayed = _isDisplayedPakcet;

}

void MainController::setCurrentPacketId(int id) {
    this->view->setCurrentSelectedPacketId(id);
}

