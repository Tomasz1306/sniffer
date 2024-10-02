//
// Created by tomasz on 02.10.24.
//

#include "models/LogModel.h"

LogModel::LogModel(std::string session_date) {
    this->session_date = session_date;
}

void LogModel::addLog(std::string date, std::string description) {
    this->logs.emplace_back(date, description);
}

std::vector<std::pair<std::string, std::string>>& LogModel::getLogs() {
    return this->logs;
}