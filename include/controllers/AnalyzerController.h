//
// Created by tomasz on 02.10.24.
//

#ifndef ANALYZERCONTROLLER_H
#define ANALYZERCONTROLLER_H

#include "models/AnalyzerModel.h"
#include "views/AnalyzerView.h"

class AnalyzerModel;
class AnalyzerView;

#include <memory>

class AnalyzerController : public std::enable_shared_from_this<AnalyzerController> {
public:
    AnalyzerController(std::shared_ptr<AnalyzerModel> model, std::shared_ptr<AnalyzerView> view);

    std::shared_ptr<AnalyzerController> getController();

    void display();

private:
    std::shared_ptr<AnalyzerModel> model;
    std::shared_ptr<AnalyzerView> view;
};

#endif //ANALYZERCONTROLLER_H
