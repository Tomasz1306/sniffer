#include "controllers/MainController.h"
#include "views/MainView.h"
#include "views/AnotherView.h"
#include "services/RenderService.h"

MainController::MainController() {
    setupViews();
}

void MainController::setupViews() {
    views.push_back(std::make_shared<MainView>());
    views.push_back(std::make_shared<AnotherView>());
}

void MainController::run() {
    RenderService renderService(views);
    renderService.render();
}
