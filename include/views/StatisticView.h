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
    std::vector<float> histogramData;
    std::vector<float> currentHistogramData;
    float interpolationSpeed;
    std::vector<ImU32> barColors = {
        IM_COL32(255, 0, 0, 255),
        IM_COL32(0, 255, 0, 255),
        IM_COL32(0, 0, 255, 255),
        IM_COL32(255, 255, 0, 255),
        IM_COL32(255, 0, 255, 255),
        IM_COL32(0, 255, 255, 255),
    };
};

#endif //STATISTICVIEW_H
