//
// Created by tomasz on 02.10.24.
//

#ifndef DATABASEMODEL_H
#define DATABASEMODEL_H

#include <vector>
#include <string>

class DataBaseModel {
public:

    DataBaseModel();

    void setHost(std::string host);
    void setPort(std::string port);
    void setUsername(std::string username);
    void setPassword(std::string password);
    void setIsConnected(bool isConnected);
    void setDate(std::string date);
    void addDatabase(std::string, bool isSelected);
    void selectDatabaseIndex(int);

    std::vector<std::pair<std::string, bool>>& getDatabases();
    std::string getHost();
    std::string getPort();
    std::string getDate();
    std::string getUsername();
    std::string getPassword();
    bool isConnected();
    int getSelectedDatabaseIndex();

    void setConnection(std::string host, std::string port, std::string username, std::string password, std::string date);

private:

    std::string m_date;
    std::string m_host;
    std::string m_port{"3306"};
    std::string m_username;
    std::string m_password;
    bool m_isConnected;
    int selectedDatabaseIndex{-1};

    std::vector<std::pair<std::string, bool>> databases;
};

#endif //DATABASEMODEL_H
