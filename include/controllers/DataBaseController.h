//
// Created by tomasz on 02.10.24.
//

#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include "cppconn/prepared_statement.h"

#include "controllers/MainController.h"
#include "models/DataBaseModel.h"
#include "views/DataBaseView.h"

#include <memory>
#include <fstream>
#include <thread>

class MainController;
class DataBaseModel;
class DataBaseView;

class DataBaseController : public std::enable_shared_from_this<DataBaseController>{
public:
    DataBaseController(std::shared_ptr<DataBaseModel> model, std::shared_ptr<DataBaseView> view);

    ~DataBaseController();

    void display();
    std::shared_ptr<DataBaseController> getController();

    void connectToDataBase(std::string host, std::string port, std::string username, std::string password);
    void disconnectFromDataBase();
    void loadDatabases();
    void newDatabase(std::string database);
    void selectDatabaseIndex(int databaseIndex);
    void createTables();
    void useDatabase();
    void insertNewPacket(CapturedPackets &packet);
    void setMainController(std::shared_ptr<MainController> controller);
    void dataBaseThread();

    bool isConnectedToDataBase();

    int getSelectedDatabaseIndex();
    std::string getHost();
    std::string getPort();
    std::string getUsername();
    std::string getPassword();
    std::vector<std::pair<std::string, bool>>& getDatabases();
    static std::string readSQLScript(const std::string& file);

private:

    std::shared_ptr<DataBaseModel> model;
    std::shared_ptr<DataBaseView> view;
    std::shared_ptr<std::thread> dbThread;


    sql::mysql::MySQL_Driver *driver;
    sql::Connection *connection;
    sql::Statement *stmt;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;

    std::shared_ptr<MainController> mainController;

};

#endif //DATABASECONTROLLER_H
