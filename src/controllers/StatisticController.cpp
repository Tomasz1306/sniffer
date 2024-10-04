//
// Created by tomasz on 01.10.24.
//

#include "controllers/StatisticController.h"

StatisticController::StatisticController(std::shared_ptr<StatisticModel> model, std::shared_ptr<StatisticView> view, std::shared_ptr<Listener> listener) {
    this->view = view;
    this->model = model;
    this->listener = listener;
}

std::shared_ptr<StatisticController> StatisticController::getController() {
    return shared_from_this();
}

void StatisticController::display() {
    this->view->draw(this->getController());
}

void StatisticController::addPacketToStatistics(pcpp::Packet &packet) {
    this->model->addPacketToStatistics(packet);
}

std::shared_ptr<StatisticModel> StatisticController::getModel() {
    return this->model;
}

bool StatisticController::isDeviceSelected() {
    return this->listener->isDeviceSelected();
}
