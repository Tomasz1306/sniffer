//
// Created by tomasz on 23.09.24.
//

#ifndef WINDOWMANAGERVIEW_H
#define WINDOWMANAGERVIEW_H

#include "views/View.h"
#include "controllers/WindowManagerController.h"
#include <vector>
#include <memory>
#include "controllers/WindowManagerController.h"

class WindowManagerController;

class WindowManagerView : public View {
public:
    WindowManagerView();
    void draw(std::shared_ptr<WindowManagerController> controller);

private:


};

#endif //WINDOWMANAGERVIEW_H
