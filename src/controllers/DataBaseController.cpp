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
    delete this->driver;
    delete this->connection;
    delete this->stmt;
    delete this->res;
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
            if (this->res->getString(1).find("sniffer_") != std::string::npos) {
                this->model->addDatabase(res->getString(1), false);
            }
        }
        LogController::getInstance()->addLog(Utils::getTime(), "LOADED DATABASES", LogType::SUCCESFULL);
    } catch (sql::SQLException& e) {
        LogController::getInstance()->addLog(Utils::getTime(), e.what(), LogType::WARNING);
    }
}

void DataBaseController::newDatabase(std::string database) {
    try {
        std::string query = std::string("CREATE DATABASE IF NOT EXISTS ") + std::string("sniffer_") + database;
        this->stmt = this->connection->createStatement();
        this->stmt->execute(query);
        LogController::getInstance()->addLog(Utils::getTime(), "CREATED NEW DATABASE", LogType::SUCCESFULL);
    } catch (sql::SQLException& e) {
        LogController::getInstance()->addLog(Utils::getTime(), e.what(), LogType::WARNING);
    }
}

void DataBaseController::selectDatabaseIndex(int databaseIndex) {
    this->model->selectDatabaseIndex(databaseIndex);
}

void DataBaseController::createTables() {
    try {
        this->stmt = this->connection->createStatement();
        stmt->execute(readSQLScript("create_table_arp.sql"));
        stmt->execute(readSQLScript("create_table_dhcpv4.sql"));
        stmt->execute(readSQLScript("create_table_dhcpv6.sql"));
        stmt->execute(readSQLScript("create_table_dns.sql"));
        stmt->execute(readSQLScript("create_table_ethernet.sql"));
        stmt->execute(readSQLScript("create_table_ftp.sql"));
        stmt->execute(readSQLScript("create_table_http.sql"));
        stmt->execute(readSQLScript("create_table_icmpv4.sql"));
        stmt->execute(readSQLScript("create_table_icmpv6.sql"));
        stmt->execute(readSQLScript("create_table_ipv4.sql"));
        stmt->execute(readSQLScript("create_table_ipv6.sql"));
        stmt->execute(readSQLScript("create_table_smtp.sql"));
        stmt->execute(readSQLScript("create_table_ssh.sql"));
        stmt->execute(readSQLScript("create_table_tcp.sql"));
        stmt->execute(readSQLScript("create_table_telnet.sql"));
        stmt->execute(readSQLScript("create_table_udp.sql"));
        stmt->execute(readSQLScript("create_table_Packets.sql"));
        stmt->execute(readSQLScript("create_table_interfaces.sql"));
        stmt->execute(readSQLScript("create_table_Session.sql"));


        LogController::getInstance()->addLog(Utils::getTime(), "CREATED ALL TABLES", LogType::SUCCESFULL);
    } catch (sql::SQLException& e) {
        LogController::getInstance()->addLog(Utils::getTime(), e.what(), LogType::ERROR);
    }
}

void DataBaseController::useDatabase() {
    try {
        this->stmt = this->connection->createStatement();
        stmt->execute("USE " + this->model->getDatabases()[this->model->getSelectedDatabaseIndex()].first);
        LogController::getInstance()->addLog(Utils::getTime(), "USING DATABASE: " + this->model->getDatabases()[this->model->getSelectedDatabaseIndex()].first, LogType::SUCCESFULL);
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

std::vector<std::pair<std::string, bool>>& DataBaseController::getDatabases() {
    return this->model->getDatabases();
}

bool DataBaseController::isConnectedToDataBase() {
    return this->model->isConnected();
}

int DataBaseController::getSelectedDatabaseIndex() {
    return this->model->getSelectedDatabaseIndex();
}

std::string DataBaseController::readSQLScript(const std::string &file) {
    std::ifstream fileStream("../sql/" + file);
    if (!fileStream.is_open()) {
        LogController::getInstance()->addLog(Utils::getTime(), "Could not open sql script: " + file, LogType::WARNING);
        return "";
    }

    std::stringstream script;
    script << fileStream.rdbuf();
    return script.str();
}


