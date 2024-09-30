//
// Created by tomasz on 30.09.24.
//

#include "models/WindowManagerModel.h"

WindowManagerModel::WindowManagerModel() {

}

std::vector<std::shared_ptr<View>> WindowManagerModel::getViews() {
    return this->views;
}

std::shared_ptr<View> WindowManagerModel::getCurrentView() {
    return this->currentView;
}

void WindowManagerModel::addView(std::shared_ptr<View> view) {
    this->views.push_back(view);
}
