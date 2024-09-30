//
// Created by tomasz on 23.09.24.
//

#ifndef WINDOWMANAGERCONTROLLER_H
#define WINDOWMANAGERCONTROLLER_H

#include <memory>
#include "views/View.h"
#include "views/WindowManagerView.h"
#include "models/WindowManagerModel.h"

class WindowManagerView;
class WindowManagerModel;

class WindowManagerController : public std::enable_shared_from_this<WindowManagerController>{
public:
    WindowManagerController(std::shared_ptr<WindowManagerModel> model, std::shared_ptr<WindowManagerView> view);
    void display();
    std::shared_ptr<WindowManagerController> getController();
    std::vector<std::shared_ptr<View>> getViews();
    std::shared_ptr<View> getCurrentView();
    void addView(std::shared_ptr<View> view);
private:
    std::shared_ptr<WindowManagerModel> model;
    std::shared_ptr<WindowManagerView> view;
};

#endif //WINDOWMANAGERCONTROLLER_H
