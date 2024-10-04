//
// Created by tomasz on 02.10.24.
//

#include "controllers/DataBaseController.h"
#include "controllers/LogController.h"
#include "utils/Utils.h"

DataBaseController::DataBaseController(std::shared_ptr<DataBaseModel> model, std::shared_ptr<DataBaseView> view) {
    this->model = model;
    this->view = view;

    this->driver = sql::mysql::get_mysql_driver_instance();
}

DataBaseController::~DataBaseController() {
    if (this->driver != nullptr) {
        delete this->driver;
    }
    if (this->connection != nullptr) {
        delete this->connection;
    }
    if (this->stmt != nullptr) {
        delete this->stmt;
    }
    if (this->res != nullptr) {
        delete this->res;
    }
}


std::shared_ptr<DataBaseController> DataBaseController::getController() {
    return shared_from_this();
}

void DataBaseController::display() {
    this->view->draw(shared_from_this());
}

void DataBaseController::connectToDataBase(std::string host, std::string port, std::string username, std::string password) {
    try {
        this->connection = this->driver->connect(host + ":" + port, username, password);
        LogController::getInstance()->addLog(Utils::getTime(), "Connected to database succesfully", LogType::SUCCESFULL);
        this->model->setConnection(host, port, username, password, Utils::getTime());
        this->model->setIsConnected(true);
    } catch(sql::SQLException& e) {
        LogController::getInstance()->addLog(Utils::getTime(), e.what(), LogType::ERROR);
    }
}

void DataBaseController::disconnectFromDataBase() {

}

void DataBaseController::loadDatabases() {
    try {
        this->stmt = this->connection->createStatement();
        this->res = this->stmt->executeQuery("SHOW DATABASES");
        this->model->getDatabases().clear();
        while (this->res->next()) {
            this->model->addDatabase(res->getString(1));
        }
        LogController::getInstance()->addLog(Utils::getTime(), "LOADED DATABASES", LogType::SUCCESFULL);
    } catch (sql::SQLException& e) {
        LogController::getInstance()->addLog(Utils::getTime(), e.what(), LogType::WARNING);
    }
}

void DataBaseController::newDatabase(std::string database) {
    try {
        this->prep_stmt = this->connection->prepareStatement("CREATE DATABASE IF NOT EXISTS ?");
        this->prep_stmt->setString(1, database);
        this->prep_stmt->execute();
        LogController::getInstance()->addLog(Utils::getTime(), "CREATED NEW DATABASE", LogType::SUCCESFULL);
    } catch (sql::SQLException& e) {
        LogController::getInstance()->addLog(Utils::getTime(), e.what(), LogType::WARNING);
    }
}

std::string DataBaseController::getHost() {
    return this->model->getHost();
}

std::string DataBaseController::getPort() {
    return this->model->getPort();
}

std::string DataBaseController::getUsername() {
    return this->model->getUsername();
}

std::string DataBaseController::getPassword() {
    return this->model->getPassword();
}

std::vector<std::string>& DataBaseController::getDatabases() {
    return this->model->getDatabases();
}

bool DataBaseController::isConnectedToDataBase() {
    return this->model->isConnected();
}
