//
// Created by tomasz on 02.10.24.
//

#ifndef LOGCONTROLLER_H
#define LOGCONTROLLER_H

#include "models/LogModel.h"
#include "views/LogView.h"

#include <memory>
#include <vector>

class LogModel;
class LogView;

class LogController : public std::enable_shared_from_this<LogController> {
public:
    LogController();

    static std::shared_ptr<LogController> getInstance();

    void display();
    void setModel(std::shared_ptr<LogModel> model);
    void setView(std::shared_ptr<LogView> view);

    void addLog(std::string date, std::string description, LogType type);
    std::vector<LogData>& getLogs();

private:

    std::shared_ptr<LogModel> model;
    std::shared_ptr<LogView> view;

};

#endif //LOGCONTROLLER_H
