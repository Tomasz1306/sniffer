//
// Created by tomasz on 23.09.24.
//

#include "controllers/WindowManagerController.h"

WindowManagerController::WindowManagerController(std::shared_ptr<WindowManagerModel> model, std::shared_ptr<WindowManagerView> view) {
    this->model = model;
    this->view = view;
}

void WindowManagerController::display() {
    this->view->draw(this->getController());
}

std::shared_ptr<WindowManagerController> WindowManagerController::getController() {
    return shared_from_this();
}

std::vector<std::shared_ptr<View>> WindowManagerController::getViews() {
    return this->model->getViews();
}

std::shared_ptr<View> WindowManagerController::getCurrentView() {
    return this->model->getCurrentView();
}

void WindowManagerController::addView(std::shared_ptr<View> view) {
    this->model->addView(view);
}