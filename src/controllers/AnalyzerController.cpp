//
// Created by tomasz on 02.10.24.
//

#include "controllers/AnalyzerController.h"
AnalyzerController::AnalyzerController(std::shared_ptr<AnalyzerModel> model, std::shared_ptr<AnalyzerView> view) {
    this->model = model;
    this->view = view;
}
std::shared_ptr<AnalyzerController> AnalyzerController::getController() {
    return shared_from_this();
}

void AnalyzerController::display() {
    this->view->draw(shared_from_this());
}
