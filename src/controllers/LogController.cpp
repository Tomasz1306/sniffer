//
// Created by tomasz on 02.10.24.
//

#include "controllers/LogController.h"

std::shared_ptr<LogController> logControllerInstance = nullptr;

LogController::LogController() {}

std::shared_ptr<LogController> LogController::getInstance() {
    if (logControllerInstance != nullptr) {
        return logControllerInstance;
    } else {
        return logControllerInstance = std::make_shared<LogController>();
    }
}

void LogController::display() {
    this->view->draw(logControllerInstance);
}

void LogController::addLog(std::string date, std::string description) {
    this->model->addLog(date, description);
}

void LogController::setModel(std::shared_ptr<LogModel> model) {
    this->model = model;
}

void LogController::setView(std::shared_ptr<LogView> view) {
    this->view = view;
}