//
// Created by tomasz on 8/10/24.
//
#include "controllers/FilterController.h"
#include "controllers/LogController.h"
#include "utils/Utils.h"
#include <iostream>

FilterController::FilterController(std::shared_ptr<FilterModel> _model, std::shared_ptr<FilterView> _view, std::shared_ptr<Listener> _listener) {
    this->thread_update = std::make_shared<std::thread>(&FilterController::update, this);
    this->model = _model;
    this->view = _view;
    this->listener = _listener;
}
void FilterController::run() {

}

void FilterController::update(){
    while(1) {
        if (this->model != nullptr && this->model->getUpdate()) {
            pcpp::AndFilter andFilter;
            pcpp::OrFilter orFilterIpAddress;
            pcpp::OrFilter orFilterMacAddress;
            uint8_t tcpFlags = 0;
            if (!this->model->getIpFilterVector().empty()) {
                for (auto &ip : this->model->getIpFilterVector()) {
                    orFilterIpAddress.addFilter(&ip.second);
                }
                andFilter.addFilter(&orFilterIpAddress);
            }
            if (!this->model->getMacFilterVector().empty()) {
                for (auto &mac : this->model->getMacFilterVector()) {
                    orFilterMacAddress.addFilter(&mac.second);
                }
                andFilter.addFilter(&orFilterMacAddress);
            }
            for (auto &tcpFlag : this->model->getTcpFlagsVector()) {
                if (tcpFlag.first == "FIN") {
                    if (*tcpFlag.second) {
                        tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpFin;
                    }
                }
                else if (tcpFlag.first == "SYN") {
                    if (*tcpFlag.second) {
                        tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpSyn;
                    }
                }
                else if (tcpFlag.first == "ACK") {
                    if (*tcpFlag.second) {
                        tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpAck;
                    }
                }
                else if (tcpFlag.first == "PSH") {
                    if (*tcpFlag.second) {
                        tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpPush;
                    }
                }
                else if (tcpFlag.first == "RST") {
                    if (*tcpFlag.second) {
                        tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpRst;
                    }
                }
                else if (tcpFlag.first == "URG") {
                    if (*tcpFlag.second) {
                        tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpUrg;
                    }
                }
            }
            pcpp::TcpFlagsFilter tcpFlagsFilter(tcpFlags, pcpp::TcpFlagsFilter::MatchOneAtLeast);
            andFilter.addFilter(&tcpFlagsFilter);
            std::string filterString;

            andFilter.parseToString(filterString);
            std::cout << filterString << std::endl;
            this->actualBfsFilter = filterString;
            this->listener->setFilters(andFilter);
            LogController::getInstance()->addLog(Utils::getTime(), filterString, LogType::SUCCESFULL);
            this->model->setUpdate(false);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void FilterController::display(){
    this->view->draw(shared_from_this(), this->model);
}

void FilterController::setMainController(std::shared_ptr<MainController> controller) {
    this->mainController = controller;
}

bool FilterController::isDeviceOpen() {
    if (this->mainController == nullptr) {
        return false;
    }
    return this->mainController->isDeviceOpen();
}
