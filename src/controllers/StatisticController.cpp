//
// Created by tomasz on 01.10.24.
//

#include "controllers/StatisticController.h"

StatisticController::StatisticController(std::shared_ptr<StatisticModel> model, std::shared_ptr<StatisticView> view) {
    this->view = view;
    this->model = model;
}

std::shared_ptr<StatisticController> StatisticController::getController() {
    return shared_from_this();
}

void StatisticController::display() {
    this->view->draw(this->getController());
}
