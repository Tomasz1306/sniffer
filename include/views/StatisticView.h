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
        IM_COL32(255, 0, 0, 255),  // Red
        IM_COL32(0, 255, 0, 255),  // Green
        IM_COL32(0, 0, 255, 255),  // Blue
        IM_COL32(255, 255, 0, 255), // Yellow
        IM_COL32(255, 0, 255, 255), // Magenta
        IM_COL32(0, 255, 255, 255), // Cyan
        // Add more colors if needed, ensuring the length matches histogramData.size()
    };
};

#endif //STATISTICVIEW_H
