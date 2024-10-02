//
// Created by tomasz on 01.10.24.
//

#ifndef STATISTICVIEW_H
#define STATISTICVIEW_H

#include "views/View.h"
#include "controllers/StatisticController.h"

#include <memory>

class StatisticController;

class StatisticView : public View {
public:
    StatisticView();
    void draw(std::shared_ptr<StatisticController> controller);

private:

};

#endif //STATISTICVIEW_H
