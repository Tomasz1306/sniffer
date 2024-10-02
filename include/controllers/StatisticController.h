//
// Created by tomasz on 01.10.24.
//

#ifndef STATISTICCONTROLLER_H
#define STATISTICCONTROLLER_H
#include <memory>

#include "models/StatisticModel.h"
#include "views/StatisticView.h"

class StatisticModel;
class StatisticView;

class StatisticController : public std::enable_shared_from_this<StatisticController> {
public:
    StatisticController(std::shared_ptr<StatisticModel> model, std::shared_ptr<StatisticView> view);
    std::shared_ptr<StatisticController> getController();
    void display();
private:
    std::shared_ptr<StatisticModel> model;
    std::shared_ptr<StatisticView> view;
};

#endif //STATISTICCONTROLLER_H
