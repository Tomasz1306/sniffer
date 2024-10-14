//
// Created by tomasz on 02.10.24.
//

#ifndef ANALYZERVIEW_H
#define ANALYZERVIEW_H

#include "controllers/AnalyzerController.h"
#include "views/View.h"

#include <memory>

class AnalyzerController;

class AnalyzerView : public View {
public:
    AnalyzerView();

    void draw(std::shared_ptr<AnalyzerController> controller);

private:
    bool isAnalyzingEnable = false;
};

#endif //ANALYZERVIEW_H
