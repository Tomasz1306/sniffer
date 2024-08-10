//
// Created by tomasz on 8/10/24.
//
#include "controllers/FilterController.h"

FilterController::FilterController(std::shared_ptr<FilterModel> _model, std::shared_ptr<FilterView> _view) {
    this->model = _model;
    this->view = _view;
}
void FilterController::run() {

}

void FilterController::update(){

}

void FilterController::display(){
    this->view->draw();
}