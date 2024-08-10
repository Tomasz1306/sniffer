//
// Created by tomasz on 8/10/24.
//

#ifndef FILTERCONTROLLER_H
#define FILTERCONTROLLER_H
#include "models/FilterModel.h"
#include "views/FilterView.h"

#include <memory>

class FilterController {
public:

    FilterController(std::shared_ptr<FilterModel> _model, std::shared_ptr<FilterView> _view);
    void run();
    void update();
    void display();

private:

    std::shared_ptr<FilterView> view;
    std::shared_ptr<FilterModel> model;

};

#endif //FILTERCONTROLLER_H
