//
// Created by tomasz on 02.10.24.
//

#include "models/LogModel.h"

LogModel::LogModel(std::string session_date) {
    this->session_date = session_date;
}

void LogModel::addLog(std::string date, std::string description, LogType type) {
    this->logs.emplace_back(date, description, type);
}

std::vector<LogData>& LogModel::getLogs() {
    return this->logs;
}
const std::vector<LogData>& LogModel::getLogsConst() {
    return this->logs;
}