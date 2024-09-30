//
// Created by tomasz on 30.09.24.
//

#ifndef WINDOWMANGERMODEL_H
#define WINDOWMANGERMODEL_H

#include "views/View.h"
#include <memory>
#include <vector>

class WindowManagerModel {
public:
    WindowManagerModel();
    std::vector<std::shared_ptr<View>> getViews();
    std::shared_ptr<View> getCurrentView();
    void addView(std::shared_ptr<View> view);
private:
    std::vector<std::shared_ptr<View>> views;
    std::shared_ptr<View> currentView{nullptr};
};

#endif //WINDOWMANGERMODEL_H
