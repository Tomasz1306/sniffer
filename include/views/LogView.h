//
// Created by tomasz on 02.10.24.
//

#ifndef LOGVIEW_H
#define LOGVIEW_H

#include "views/View.h"
#include "controllers/LogController.h"

#include <memory>

class LogController;

class LogView : public View {
public:
    LogView();
    void draw(std::shared_ptr<LogController> controller);
private:

};

#endif //LOGVIEW_H
