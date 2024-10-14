//
// Created by tomasz on 8/10/24.
//

#ifndef FILTERCONTROLLER_H
#define FILTERCONTROLLER_H
#include "models/FilterModel.h"
#include "views/FilterView.h"
#include "listeners/Listener.h"
#include "controllers/MainController.h"

#include <memory>
#include <thread>

class MainController;
class FilterModel;
class FilterView;

class FilterController : public std::enable_shared_from_this<FilterController>{
public:

    FilterController(std::shared_ptr<FilterModel> _model, std::shared_ptr<FilterView> _view, std::shared_ptr<Listener> _listener);
    void run();
    void update();
    void display();
    void setMainController(std::shared_ptr<MainController> controller);
    bool isDeviceOpen();

private:
    std::string actualBfsFilter;
    std::shared_ptr<std::thread> thread_update;
    std::shared_ptr<FilterView> view;
    std::shared_ptr<FilterModel> model;
    std::shared_ptr<Listener> listener;
    std::shared_ptr<MainController> mainController;

};

#endif //FILTERCONTROLLER_H
