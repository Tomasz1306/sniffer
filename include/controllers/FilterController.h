//
// Created by tomasz on 8/10/24.
//

#ifndef FILTERCONTROLLER_H
#define FILTERCONTROLLER_H
#include "models/FilterModel.h"
#include "views/FilterView.h"
#include "listeners/Listener.h"

#include <memory>
#include <thread>

#include <PcapFilter.h>

class FilterController {
public:

    FilterController(std::shared_ptr<FilterModel> _model, std::shared_ptr<FilterView> _view, std::shared_ptr<Listener> _listener);
    void run();
    void update();
    void display();

private:
    std::string actualBfsFilter;
    std::shared_ptr<std::thread> thread_update;
    std::shared_ptr<FilterView> view;
    std::shared_ptr<FilterModel> model;
    std::shared_ptr<Listener> listener;

};

#endif //FILTERCONTROLLER_H
