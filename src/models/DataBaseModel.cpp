//
// Created by tomasz on 02.10.24.
//

#include "models/DataBaseModel.h"

DataBaseModel::DataBaseModel() {

}

std::string DataBaseModel::getHost() {
    return this->m_host;
}

std::string DataBaseModel::getPort() {
    return this->m_port;
}

std::string DataBaseModel::getUsername() {
    return this->m_username;
}

std::string DataBaseModel::getDate() {
    return this->m_date;
}

std::string DataBaseModel::getPassword() {
    return this->m_password;
}

std::vector<std::pair<std::string, bool>>& DataBaseModel::getDatabases() {
    return this->databases;
}

int DataBaseModel::getSelectedDatabaseIndex() {
    return this->selectedDatabaseIndex;
}


void DataBaseModel::setDate(std::string date) {
    this->m_date = date;
}

void DataBaseModel::setHost(std::string host) {
    this->m_host = host;
}

void DataBaseModel::setPort(std::string port) {
    this->m_port = port;
}

void DataBaseModel::setUsername(std::string username) {
    this->m_username = username;
}

void DataBaseModel::setPassword(std::string password) {
    this->m_password = password;
}

void DataBaseModel::setConnection(std::string host, std::string port, std::string username, std::string password, std::string date) {
    this->m_host = host;
    this->m_port = port;
    this->m_username = username;
    this->m_password = password;
    this->m_date = date;
}

bool DataBaseModel::isConnected() {
    return this->m_isConnected;
}

void DataBaseModel::setIsConnected(bool isConnected) {
    this->m_isConnected = isConnected;
}

void DataBaseModel::addDatabase(std::string database, bool isSelected) {
    this->databases.emplace_back(database, isSelected);
}

void DataBaseModel::selectDatabaseIndex(int selectedIndex) {
    this->selectedDatabaseIndex = selectedIndex;
}




