//
// Created by tomasz on 02.10.24.
//

#ifndef LOGMODEL_H
#define LOGMODEL_H

#include <string>
#include <vector>

enum class LogType {
    SUCCESFULL = 0,
    WARNING,
    ERROR
};

class LogData{
public:
    LogData(std::string date, std::string description, LogType type) :
    date(date),
    description(description),
    type(type) {}
    std::string date;
    std::string description;
    LogType type;

    bool operator==(const LogData& other) const {
        return this->date == other.date &&
            this->description == other.description &&
               this->type == other.type;
    }
};

class LogModel {
public:
    LogModel(std::string session_date);

    void addLog(std::string date, std::string description, LogType type);
    std::vector<LogData> &getLogs();
    const std::vector<LogData> &getLogsConst();

private:
    std::string session_date;
    std::vector<LogData> logs;
};

#endif //LOGMODEL_H
