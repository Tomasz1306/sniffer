#include "controllers/MainController.h"
#include "views/PacketCaptureView.h"
#include "views/PacketCaptureView.h"

MainController::MainController(std::shared_ptr<PacketCaptureModel> _model,
    std::shared_ptr<PacketCaptureView> _view,
    std::shared_ptr<Listener> _listener,
    std::shared_ptr<PacketView> _packetView,
    std::shared_ptr<StatisticController> _statisticController) {
    this->model = _model;
    this->view = _view;
    this->listener = _listener;
    this->packetView = _packetView;
    this->statisticController = _statisticController;
}

void MainController::onPacketArrivesBlockingMode(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie){
    auto model = static_cast<std::shared_ptr<PacketCaptureModel>*>(cookie);
    (*model)->addToCapturedPacketDeque(pcpp::Packet(packet));
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
    this->model->clearCapturedPacketVector();
}

bool MainController::isDeviceOpen() {
    return this->listener->isDeviceOpen();
}

void MainController::setIsDisplayedPakcet(bool _isDisplayedPakcet) {
    this->isPacketDisplayed = _isDisplayedPakcet;

}

void MainController::setCurrentPacketId(int _id) {
    this->view->setCurrentSelectedPacketId(_id);
}

void MainController::displayNextPacket() {
    if ((this->view->getCurrentSelectedPacketId() + 1) >= this->model->getCapturedPacketVector().size()) {
        return;
    }
    this->model->getCapturedPacketVector()[this->view->getCurrentSelectedPacketId()].isOpen = false;
    this->model->getCapturedPacketVector()[this->view->getCurrentSelectedPacketId() + 1].isOpen = true;
    this->view->setCurrentSelectedPacketId(this->view->getCurrentSelectedPacketId() + 1);
}

void MainController::displayPrevPacket() {
    if ((this->view->getCurrentSelectedPacketId() - 1) < 0) {
        return;
    }
    this->model->getCapturedPacketVector()[this->view->getCurrentSelectedPacketId()].isOpen = false;
    this->model->getCapturedPacketVector()[this->view->getCurrentSelectedPacketId() - 1].isOpen = true;
    this->view->setCurrentSelectedPacketId(this->view->getCurrentSelectedPacketId() - 1);
}

bool MainController::isDisplayedPakcet() {
    return this->isPacketDisplayed;
}

int MainController::getPacketCapturedVectorSize() {
    return this->model->getCapturedPacketVector().size();
}

void MainController::addPacketToStatistics(pcpp::Packet &packet) {
    this->statisticController->addPacketToStatistics(packet);
}

bool MainController::isDeviceSelected() {
    return this->listener->isDeviceSelected();
}