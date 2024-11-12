#include "controllers/MainController.h"

#include <iostream>
#include <mutex>

#include "controllers/LogController.h"
#include "views/PacketCaptureView.h"
#include "views/PacketCaptureView.h"
#include "global/Global.h"
#include "utils/Utils.h"

MainController::MainController(std::shared_ptr<PacketCaptureModel> _model,
    std::shared_ptr<PacketCaptureView> _view,
    std::shared_ptr<Listener> _listener,
    std::shared_ptr<PacketView> _packetView,
    std::shared_ptr<AnalyzerController> _analyzerController,
    std::shared_ptr<DataBaseController> _databaseController,
    std::shared_ptr<FilterController> _filterController,
    std::shared_ptr<StatisticController> _statisticController,
    std::shared_ptr<DeviceController> _deviceController) {
    this->model = _model;
    this->view = _view;
    this->listener = _listener;
    this->packetView = _packetView;
    this->analyzerController = _analyzerController;
    this->databaseController = _databaseController;
    this->filterController = _filterController;
    this->statisticController = _statisticController;
    this->deviceController = _deviceController;
}

void MainController::onPacketArrivesBlockingMode(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie){
    auto model = static_cast<std::shared_ptr<PacketCaptureModel>*>(cookie);
    if (packet->getRawDataLen() <= 65535) {
        try {
            std::lock_guard<std::mutex> lock(guard_3);
            pcpp::Packet parsedPacket(packet);
            (*model)->addToCapturedPacketDeque(parsedPacket);
        } catch (const std::exception& e) {
            LogController::getInstance()->addLog(Utils::getTime(), "Error while adding packet to vector", LogType::WARNING);
        }
    }
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
    this->model->clearCapturedPacketVectorDatabase();
    this->statisticController->clearStatistics();
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

bool *MainController::getIsPacketDisplayed() {
    return &this->isPacketDisplayed;
}

std::vector<CapturedPackets> &MainController::getCapturedPacketVectorDatabase() {
    return this->model->getCapturedPacketVectorDatabase();
}

std::string MainController::getSessionData() {
    return this->statisticController->getModel()->date;
}

std::shared_ptr<DeviceController> MainController::getDeviceController() {
    return this->deviceController;
}

std::vector<CapturedPackets> &MainController::getCapturedVectorData() {
    return this->model->getCapturedPacketVector();
}

bool MainController::isCapturing() {
    return this->listener->isDeviceCapturing();
}
std::vector<CapturedPackets> MainController::getCapturedVectorDataCopy() {
    return this->model->getCapturedPacketVector();
}

std::string MainController::getDeviceName() {
    return this->deviceController->getCurrentDeviceName();
}

int MainController::getDeviceId(std::string name) {
    return this->deviceController->getDeviceId(name);
}

std::vector<CapturedPackets> &MainController::getCapturedVectorToAnalyze() {
    return this->model->getCapturedPacketToAnalyze();
}


