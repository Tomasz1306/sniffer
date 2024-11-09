//
// Created by tomasz on 02.10.24.
//

#ifndef DATABASECONTROLLER_H
#define DATABASECONTROLLER_H

#include "controllers/MainController.h"
#include "models/DataBaseModel.h"
#include "views/DataBaseView.h"

#include "IPv4Layer.h"
#include "IPv6Layer.h"
#include "EthLayer.h"
#include "ArpLayer.h"
#include "DnsLayer.h"
#include "IcmpLayer.h"
#include "FtpLayer.h"
#include "TcpLayer.h"
#include "TelnetLayer.h"
#include "HttpLayer.h"
#include "DhcpLayer.h"
#include "UdpLayer.h"
#include "SSHLayer.h"

#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include "cppconn/prepared_statement.h"

#include <memory>
#include <thread>
#include <future>

class MainController;
class DataBaseModel;
class DataBaseView;

class DataBaseController : public std::enable_shared_from_this<DataBaseController>{
public:
    DataBaseController(std::shared_ptr<DataBaseModel> model, std::shared_ptr<DataBaseView> view);

    ~DataBaseController();
    const int numThreads = std::thread::hardware_concurrency();
    void display();
    std::shared_ptr<DataBaseController> getController();

    void connectToDataBase(std::string host, std::string port, std::string username, std::string password);
    void disconnectFromDataBase();
    void loadDatabases();
    void newDatabase(std::string database);
    void selectDatabaseIndex(int databaseIndex);
    void createTables();
    void useDatabase();
    void insertNewPacket(CapturedPackets &packet, sql::Connection *connection);
    void setMainController(std::shared_ptr<MainController> controller);
    void dataBaseThread();
    void initializeDbThreadPool();
    void stopDbThreadPool();

    void buildIpv4Query(CapturedPackets &packet, std::shared_ptr<pcpp::IPv4Layer> &layer, bool &firstField, int &ipv4_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildIpv6Query(CapturedPackets &packet, std::shared_ptr<pcpp::IPv6Layer> &layer, bool &firstField, int &ipv6_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildArpQuery(CapturedPackets &packet, std::shared_ptr<pcpp::ArpLayer> &layer, bool &firstField, int &arp_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildIcmpQuery(CapturedPackets &packet, std::shared_ptr<pcpp::IcmpLayer> &layer, bool &firstField, int &icmp_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildTcpQuery(CapturedPackets &packet, std::shared_ptr<pcpp::TcpLayer> &layer, bool &firstField, int &tcp_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildUdpQuery(CapturedPackets &packet, std::shared_ptr<pcpp::UdpLayer> &layer, bool &firstField, int &udp_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildEthernetQuery(CapturedPackets &packet, std::shared_ptr<pcpp::EthLayer> &layer, bool &firstField, int &ethernet_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    // void buildSshQuery(CapturedPackets &packet, std::shared_ptr<pcpp::SSHLayer> &layer, bool firstField, int &ssh_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    // void buildSmtpQuery(CapturedPackets &packet, std::shared_ptr<pcpp::SmtpLayer> &layer, bool firstField, int &smtp_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildTelnetQuery(CapturedPackets &packet, std::shared_ptr<pcpp::TelnetLayer> &layer, bool &firstField, int &telnet_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildDhcpv4Query(CapturedPackets &packet, std::shared_ptr<pcpp::DhcpLayer> &layer, bool &firstField, int &dhcpv4_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildHttpRequestQuery(CapturedPackets &packet, std::shared_ptr<pcpp::HttpRequestLayer> &layer, bool &firstField, int &httpRequest_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildHttpResponseQuery(CapturedPackets &packet, std::shared_ptr<pcpp::HttpResponseLayer> &layer, bool &firstField, int &httpResponse_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildDnsQuery(CapturedPackets &packet, std::shared_ptr<pcpp::DnsLayer> &layer, bool &firstField, int &dns_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildFtpRequestQuery(CapturedPackets &packet, std::shared_ptr<pcpp::FtpRequestLayer> &layer, bool &firstField, int &ftpRequest_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);
    void buildFtpResponseQuery(CapturedPackets &packet, std::shared_ptr<pcpp::FtpResponseLayer> &layer, bool &firstField, int &ftpResponse_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res);

    bool isConnectedToDataBase();

    int getSelectedDatabaseIndex();
    std::string getHost();
    std::string getPort();
    std::string getUsername();
    std::string getPassword();
    std::vector<std::pair<std::string, bool>>& getDatabases();
    static std::string readSQLScript(const std::string& file);
    float getMaxVectorSize() {return this->maxVectorSize;};
    int getCapturedPacketVectorDatabaseSize();
    int getNumberOfThreads();

private:

    const float maxVectorSize = 10000.0f;

    std::shared_ptr<DataBaseModel> model;
    std::shared_ptr<DataBaseView> view;
    std::shared_ptr<std::thread> dbThread;

    std::map<int, std::string> currentSessionInterfacesIds;

    sql::mysql::MySQL_Driver *driver;
    sql::Connection *connection;
    sql::Statement *stmt;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;

    std::shared_ptr<MainController> mainController;

    std::vector<std::thread> dbThreadPool;
    std::shared_ptr<std::condition_variable> cv = nullptr;
    std::shared_ptr<std::mutex> mutex = nullptr;
    std::shared_ptr<std::mutex> stmtMutex = nullptr;
    bool done = false;
    bool areConnectionsForThreadInitialized = false;
    bool dbThreadPoolInitialized = false;
    std::unordered_map<std::thread::id, int> threadIds;
    std::map<int, sql::Connection*> threadConnections;
};

#endif //DATABASECONTROLLER_H
