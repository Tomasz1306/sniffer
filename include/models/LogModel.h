//
// Created by tomasz on 02.10.24.
//

#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <string>
#include <vector>

class LogModel {
public:
    LogModel(std::string session_date);

    void addLog(std::string date, std::string description);
    std::vector<std::pair<std::string, std::string>> &getLogs();

private:
    std::string session_date;
    std::vector<std::pair<std::string, std::string>> logs;
};

#endif //LOGMODEL_H
