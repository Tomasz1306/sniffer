//
// Created by tomasz on 02.10.24.
//

#include "controllers/DataBaseController.h"
#include "controllers/LogController.h"
#include "utils/Utils.h"
#include "global/Global.h"



DataBaseController::DataBaseController(std::shared_ptr<DataBaseModel> model, std::shared_ptr<DataBaseView> view) {
    this->model = model;
    this->view = view;
    this->mutex = std::make_shared<std::mutex>();
    this->driver = sql::mysql::get_mysql_driver_instance();
}

DataBaseController::~DataBaseController() {
    delete this->driver;
    delete this->connection;
    delete this->stmt;
    delete this->res;
    if (this->dbThread->joinable()) {
        this->dbThread->join();
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
        for (int i = 0; i < 2; i++) {
            try {
                int session_id{0};
                auto connection = this->driver->connect(host + ":" + port, username, password);
                this->threadConnections.insert({i, connection});
            } catch (const sql::SQLException &e) {
                std::cerr << "SQLException in initializeDbThreadPool: " << e.what()
                          << ", error code: " << e.getErrorCode() << std::endl;
                LogController::getInstance()->addLog(Utils::getTime(), e.what() , LogType::ERROR);
            } catch (const std::exception &e) {
                std::cerr << "Exception in initializeDbThreadPool: " << e.what() << std::endl;
                LogController::getInstance()->addLog(Utils::getTime(), e.what() , LogType::ERROR);
            }
        }
        this->areConnectionsForThreadInitialized = true;
        LogController::getInstance()->addLog(Utils::getTime(), "Connected to database succesfully", LogType::SUCCESFULL);
        this->model->setConnection(host, port, username, password, Utils::getTime());
        this->model->setIsConnected(true);
    } catch(sql::SQLException& e) {
        LogController::getInstance()->addLog(Utils::getTime(), e.what(), LogType::ERROR);
    }
}

void DataBaseController::disconnectFromDataBase() {
    if (!this->model->isConnected()) {
        return;
    }

    if (!this->connection->isClosed()) {
        this->connection->close();
        delete this->connection;
    }
    this->stopDbThreadPool();
    for (int i = 0; i < this->threadConnections.size(); i++) {
        if (!this->threadConnections.at(i)->isClosed()) {
            this->threadConnections.at(i)->close();
            this->threadConnections.erase(i);
        }
    }
    this->model->setIsConnected(false);
}

void DataBaseController::loadDatabases() {
    if (!this->model->isConnected()) {
        return;
    }
    try {
        this->stmt = this->connection->createStatement();
        this->res = this->stmt->executeQuery("SHOW DATABASES");
        std::lock_guard<std::mutex> guard(getDatabasesMutex);
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
    if (!this->dbThreadPoolInitialized) {
        initializeDbThreadPool();
        this->dbThreadPoolInitialized = true;
    }
}

void DataBaseController::initializeDbThreadPool() {
    const int numThreads = 2;
    this->done = false;
    this->cv = std::make_shared<std::condition_variable>();

    LogController::getInstance()->addLog(Utils::getTime(), "Before Database thread started", LogType::SUCCESFULL);

    auto workerFunction = [this]() {
        std::thread::id this_thread_id = std::this_thread::get_id();
        int thread_id;
        {
            std::lock_guard<std::mutex> guard(*this->mutex);
            thread_id = this->threadIds[this_thread_id];
        }
        while (true) {
            CapturedPackets packet;
                {
                std::unique_lock<std::mutex> lock(*this->mutex);
                database_cv.wait(lock, [this]() {
                    return !this->mainController->getCapturedPacketVectorDatabase().empty() || this->done || !this->areConnectionsForThreadInitialized;
                });

                if (this->done) {
                    break;
                }

                if (!this->mainController->getCapturedPacketVectorDatabase().empty()) {
                    std::lock_guard<std::mutex> guard(guard_4);
                    packet = this->mainController->getCapturedPacketVectorDatabase().back();
                    this->mainController->getCapturedPacketVectorDatabase().pop_back();
                }
            }
            this->insertNewPacket(packet, this->threadConnections[thread_id]);
        }
    };

    try {
        for (int i = 0; i < numThreads; ++i) {

            std::thread t([this, workerFunction]() { workerFunction(); });
            {
                std::scoped_lock lock(*this->mutex, getDatabasesMutex);
                this->stmt = this->threadConnections[i]->createStatement();
                stmt->execute("USE " + this->model->getDatabases()[this->model->getSelectedDatabaseIndex()].first);
                LogController::getInstance()->addLog(Utils::getTime(), "THREAD: " + std::to_string(i)
                    + "USING DATABASE: " + this->model->getDatabases()[this->model->getSelectedDatabaseIndex()].first,
                    LogType::SUCCESFULL);
                this->threadIds[t.get_id()] = i;
            }
            this->dbThreadPool.push_back(std::move(t));
        }
    } catch (sql::SQLException& e) {
        LogController::getInstance()->addLog(Utils::getTime(), e.what(), LogType::ERROR);
    }
    LogController::getInstance()->addLog(Utils::getTime(), "Database thread started", LogType::SUCCESFULL);

    database_cv.notify_all();
}

void DataBaseController::stopDbThreadPool() {
    {
        std::unique_lock<std::mutex> lock(*this->mutex);
        this->done = true;
        database_cv.notify_all();
    }

    for (std::thread &worker : this->dbThreadPool) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    this->dbThreadPool.clear();
    this->done = false;
    this->dbThreadPoolInitialized = false;
}

int DataBaseController::getNumberOfThreads() {
    return this->dbThreadPool.size();
}


void DataBaseController::createTables() {
    auto createTableTask = [this]() {
        try {
            this->stmt = this->connection->createStatement();
            stmt->execute(readSQLScript("create_table_arp.sql"));
            stmt->execute(readSQLScript("create_table_dhcpv4.sql"));
            stmt->execute(readSQLScript("create_table_dns.sql"));
            stmt->execute(readSQLScript("create_table_ethernet.sql"));
            stmt->execute(readSQLScript("create_table_ftp_request.sql"));
            stmt->execute(readSQLScript("create_table_ftp_response.sql"));
            stmt->execute(readSQLScript("create_table_http_request.sql"));
            stmt->execute(readSQLScript("create_table_http_response.sql"));
            stmt->execute(readSQLScript("create_table_icmpv4.sql"));
            stmt->execute(readSQLScript("create_table_ipv4.sql"));
            stmt->execute(readSQLScript("create_table_ipv6.sql"));
            stmt->execute(readSQLScript("create_table_smtp.sql"));
            stmt->execute(readSQLScript("create_table_ssh.sql"));
            stmt->execute(readSQLScript("create_table_tcp.sql"));
            stmt->execute(readSQLScript("create_table_telnet.sql"));
            stmt->execute(readSQLScript("create_table_udp.sql"));
            stmt->execute(readSQLScript("create_table_Session.sql"));
            stmt->execute(readSQLScript("create_table_interfaces.sql"));
            stmt->execute(readSQLScript("create_table_Packets.sql"));

            int session_id{0};
            this->prep_stmt = this->connection->prepareStatement("INSERT INTO Sessions(session_date) VALUES(STR_TO_DATE(?, '%d-%m-%Y %H:%i:%s'))");
            this->prep_stmt->setDateTime(1, this->mainController->getSessionData());
            this->prep_stmt->execute();
            stmt = connection->createStatement();
            res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
            if (res->next()) {
                session_id = res->getInt(1);
            }
            for (auto &device : this->mainController->getDeviceController()->getDevices()) {
                std::string query = R"(INSERT INTO Interfaces(interface_name, interface_ipv4,
                                             interface_ipv6, interface_mac_address, interface_default_gateway,
                                             interface_dns_server, interface_mtu, session_id)
                                             VALUES(?,?,?,?,?,?,?,?))";
                this->prep_stmt = this->connection->prepareStatement(query);
                this->prep_stmt->setString(1, device->getName());
                this->prep_stmt->setString(2, device->getIPv4Address().toString());
                this->prep_stmt->setString(3, device->getIPv6Address().toString());
                this->prep_stmt->setString(4, device->getMacAddress().toString());
                this->prep_stmt->setString(5, device->getDefaultGateway().toString());
                this->prep_stmt->setString(6, device->getDnsServers().begin()->toString());
                this->prep_stmt->setString(7, std::to_string(device->getMtu()));
                this->prep_stmt->setInt(8, session_id);
                this->prep_stmt->execute();
                int interface_id{0};
                stmt = connection->createStatement();
                res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
                if (res->next()) {
                    interface_id = res->getInt(1);
                }
                this->currentSessionInterfacesIds.emplace(interface_id, device->getName());
            }

            LogController::getInstance()->addLog(Utils::getTime(), "CREATED ALL TABLES", LogType::SUCCESFULL);
        } catch (sql::SQLException& e) {
            LogController::getInstance()->addLog(Utils::getTime(), e.what(), LogType::ERROR);
        }
    };
    std::thread createTableThread(createTableTask);
    createTableThread.detach();
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

void DataBaseController::setMainController(std::shared_ptr<MainController> mainController) {
    this->mainController = mainController;
}

int DataBaseController::getCapturedPacketVectorDatabaseSize() {
    if (this->mainController == nullptr) {
        return 0;
    }
    return this->mainController->getCapturedPacketVectorDatabase().size();
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

void DataBaseController::insertNewPacket(CapturedPackets &packet, sql::Connection *connection_local) {
    int ipv4_id{-1}, ipv6_id{-1}, arp_id{-1}, icmp_id{-1}, index{2};
    int icmpv6_id{-1}, igmp_id{-1}, tcp_id{-1}, udp_id{-1}, ethernet_id{-1}, httpRequest_id{-1};
    int ssh_id{-1}, smtp_id{-1}, telnet_id{-1}, dhcpv4_id{-1}, dhcpv6_id{-1}, httpResponse_id{-1};
    int ftpRequest_id{-1}, ftpResponse_id{-1}, dns_id{-1};
    std::shared_ptr<pcpp::IPv4Layer> ipv4Layer;
    std::shared_ptr<pcpp::IPv6Layer> ipv6Layer;
    std::shared_ptr<pcpp::ArpLayer> arpLayer;
    std::shared_ptr<pcpp::IcmpLayer> icmpLayer;
    std::shared_ptr<pcpp::IgmpLayer> igmpLayer;
    std::shared_ptr<pcpp::TcpLayer> tcpLayer;
    std::shared_ptr<pcpp::UdpLayer> udpLayer;
    std::shared_ptr<pcpp::EthLayer> ethLayer;
    std::shared_ptr<pcpp::HttpRequestLayer> httpRequestLayer;
    std::shared_ptr<pcpp::HttpResponseLayer> httpResponseLayer;
    std::shared_ptr<pcpp::FtpRequestLayer> ftpRequestLayer;
    std::shared_ptr<pcpp::FtpResponseLayer> ftpResponseLayer;
    std::shared_ptr<pcpp::SmtpLayer> smtpLayer;
    std::shared_ptr<pcpp::TelnetLayer> telnetLayer;
    std::shared_ptr<pcpp::SSHLayer> sshLayer;
    std::shared_ptr<pcpp::DnsLayer> dnsLayer;
    std::shared_ptr<pcpp::DhcpLayer> dhcpv4Layer;
    std::string query = "";
    bool firstField = true;
    bool isProtocolSupported = false;
    std::string packetsQuery = "INSERT INTO Packets(packet_capture_date, ";
    std::string values = "VALUES(STR_TO_DATE(?,'%d-%m-%Y %H:%i:%s'), ";

    try {
        sql::Statement *stmt;
        sql::PreparedStatement *prep_stmt;
        sql::ResultSet *res;

        if (packet.packet.isPacketOfType(pcpp::Ethernet)) {
            buildEthernetQuery(packet, ethLayer, firstField, ethernet_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (packet.packet.isPacketOfType(pcpp::IPv4)) {
            buildIpv4Query(packet, ipv4Layer, firstField, ipv4_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (packet.packet.isPacketOfType(pcpp::IPv6)) {
            buildIpv6Query(packet, ipv6Layer, firstField, ipv6_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (packet.packet.isPacketOfType(pcpp::ARP)) {
            buildArpQuery(packet, arpLayer, firstField, arp_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (packet.packet.isPacketOfType(pcpp::ICMP)) {
            buildIcmpQuery(packet, icmpLayer, firstField, icmp_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (packet.packet.isPacketOfType(pcpp::TCP)) {
            buildTcpQuery(packet, tcpLayer, firstField, tcp_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (packet.packet.isPacketOfType(pcpp::UDP)) {
            buildUdpQuery(packet, udpLayer, firstField, udp_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (packet.packet.isPacketOfType(pcpp::HTTPRequest)) {
            buildHttpRequestQuery(packet, httpRequestLayer, firstField,  httpRequest_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (packet.packet.isPacketOfType(pcpp::HTTPResponse)) {
            buildHttpResponseQuery(packet, httpResponseLayer, firstField,  httpResponse_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        auto ftpLayer = packet.packet.getLayerOfType<pcpp::FtpLayer>();
        if (auto isftpRequestLayer = dynamic_cast<pcpp::FtpRequestLayer*>(ftpLayer)) {
            buildFtpRequestQuery(packet, ftpRequestLayer, firstField, ftpRequest_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        } else if (auto isftpResponseLayer = dynamic_cast<pcpp::FtpResponseLayer*>(ftpLayer)) {
            buildFtpRequestQuery(packet, ftpRequestLayer, firstField, ftpRequest_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        } else {
            //std::cout << "Unknown FTP type." << std::endl;
        }
        if (packet.packet.isPacketOfType(pcpp::SSH)) {
            // this->buildSshQuery(packet, sshLayer, firstField, ssh_id, packetsQuery, values);
        }
        if (packet.packet.isPacketOfType(pcpp::Telnet)) {
            buildTelnetQuery(packet, telnetLayer, firstField, telnet_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (packet.packet.isPacketOfType(pcpp::DNS)) {
            buildDnsQuery(packet, dnsLayer, firstField, dns_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (packet.packet.isPacketOfType(pcpp::DHCP)) {
            buildDhcpv4Query(packet, dhcpv4Layer, firstField, dhcpv4_id, packetsQuery, values, connection_local, prep_stmt, stmt, res);
            isProtocolSupported = true;
        }
        if (ethernet_id == -1 && ipv4_id == -1 && ipv6_id == -1 && arp_id == -1 && icmp_id == -1 && tcp_id == -1 && udp_id == -1 && icmpv6_id == -1) {
            return;
        }
        if (!isProtocolSupported) {
            return;
        }
        query.clear();
        query = packetsQuery + ")" + values + ")";
        prep_stmt = connection_local->prepareStatement(query);
        prep_stmt->setDateTime(1, packet.captureTime);
        if (ethernet_id != -1) {
            prep_stmt->setInt(index++, ethernet_id);
        }
        if (ipv4_id != -1) {
            prep_stmt->setInt(index++, ipv4_id);
        }
        if (ipv6_id != -1) {
            prep_stmt->setInt(index++, ipv6_id);
        }
        if (arp_id != -1) {
            prep_stmt->setInt(index++, arp_id);
        }
        if (icmp_id != -1) {
            prep_stmt->setInt(index++, icmp_id);
        }
        if (icmpv6_id != -1) {
            prep_stmt->setInt(index++, icmpv6_id);
        }
        if (tcp_id != -1) {
            prep_stmt->setInt(index++, tcp_id);
        }
        if (udp_id != -1) {
            prep_stmt->setInt(index++, udp_id);
        }

        prep_stmt->executeUpdate();

    } catch (sql::SQLException &e) {
        std::lock_guard<std::mutex> lock(logGuard);
        LogController::getInstance()->addLog(Utils::getTime(), e.what() + std::string(" ") + __PRETTY_FUNCTION__, LogType::WARNING);
    }
}

void DataBaseController::buildIpv4Query(CapturedPackets &packet, std::shared_ptr<pcpp::IPv4Layer> &layer,
    bool &firstField, int &ipv4_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection_local, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::IPv4Layer>(*packet.packet.getLayerOfType<pcpp::IPv4Layer>());
    std::string query = R"(INSERT INTO ipv4(ipv4_version, ipv4_ihl, ipv4_tos, ipv4_total_length,
                    ipv4_identification, ipv4_flags, ipv4_fragment_offset, ipv4_ttl, ipv4_protocol,
                    ipv4_header_checksum, ipv4_src_ip, ipv4_dst_ip) VALUES (?,?,?,?,?,?,?,?,?,?,?,?))";
    prep_stmt = connection_local->prepareStatement(query);
    prep_stmt->setInt(1, layer->getIPv4Header()->ipVersion);
    prep_stmt->setInt(2, layer->getIPv4Header()->internetHeaderLength);
    prep_stmt->setInt(3, layer->getIPv4Header()->typeOfService);
    prep_stmt->setInt(4, layer->getIPv4Header()->totalLength);
    prep_stmt->setInt(5, layer->getIPv4Header()->ipId);
    prep_stmt->setInt(6, layer->getFragmentFlags());
    prep_stmt->setInt(7, layer->getIPv4Header()->fragmentOffset);
    prep_stmt->setInt(8, layer->getIPv4Header()->timeToLive);
    prep_stmt->setInt(9, layer->getIPv4Header()->protocol);
    prep_stmt->setInt(10, layer->getIPv4Header()->headerChecksum);
    prep_stmt->setString(11, layer->getSrcIPAddress().toString().c_str());
    prep_stmt->setString(12, layer->getDstIPAddress().toString().c_str());
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection_local->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        ipv4_id = res->getInt(1);
    }
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "ipv4_id";
    values += "?";
}

void DataBaseController::buildIpv6Query(CapturedPackets &packet, std::shared_ptr<pcpp::IPv6Layer> &layer,
    bool &firstField, int &ipv6_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::IPv6Layer>(*packet.packet.getLayerOfType<pcpp::IPv6Layer>());
    std::string query = R"(INSERT INTO ipv6(ipv6_version, ipv6_traffic_class, ipv6_flow_label, ipv6_payload_length,
            ipv6_next_header, ipv6_hop_limit, ipv6_src_ip, ipv6_dst_ip) VALUES (?,?,?,?,?,?,?,?))";
    prep_stmt = connection->prepareStatement(query);
    prep_stmt->setInt(1, layer->getIPv6Header()->ipVersion);
    prep_stmt->setInt(2, layer->getIPv6Header()->trafficClass);
    int combined = (static_cast<int>(layer->getIPv6Header()->flowLabel[0]) << 16) |
                   (static_cast<int>(layer->getIPv6Header()->flowLabel[1]) << 8) |
                    static_cast<int>(layer->getIPv6Header()->flowLabel[2]);
    prep_stmt->setInt(3, combined);
    prep_stmt->setInt(4, layer->getIPv6Header()->payloadLength);
    prep_stmt->setInt(5, layer->getIPv6Header()->nextHeader);
    prep_stmt->setInt(6, layer->getIPv6Header()->hopLimit);
    prep_stmt->setString(7, layer->getSrcIPAddress().toString().c_str());
    prep_stmt->setString(8, layer->getDstIPAddress().toString().c_str());
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        ipv6_id = res->getInt(1);
    }
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "ipv6_id";
    values += "?";
}

void DataBaseController::buildArpQuery(CapturedPackets &packet, std::shared_ptr<pcpp::ArpLayer> &layer,
    bool &firstField, int &arp_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::ArpLayer>(*packet.packet.getLayerOfType<pcpp::ArpLayer>());
    std::string query = R"(INSERT INTO arp(arp_hw_type, arp_proto_type, arp_hw_size, arp_proto_size,
            arp_opcode, arp_src_mac, arp_src_ip, arp_dst_mac, arp_dst_ip) VALUES (?,?,?,?,?,?,?,?,?))";
    prep_stmt = connection->prepareStatement(query);
    prep_stmt->setInt(1, static_cast<int>(layer->getArpHeader()->hardwareType));
    prep_stmt->setInt(2, static_cast<int>(layer->getArpHeader()->protocolType));
    prep_stmt->setInt(3, static_cast<int>(layer->getArpHeader()->hardwareSize));
    prep_stmt->setInt(4, static_cast<int>(layer->getArpHeader()->protocolSize));
    prep_stmt->setInt(5, static_cast<int>(layer->getArpHeader()->opcode));
    prep_stmt->setString(6, layer->getSenderMacAddress().toString().c_str());
    prep_stmt->setString(7, layer->getSenderIpAddr().toString().c_str());
    prep_stmt->setString(8, layer->getTargetMacAddress().toString().c_str());
    prep_stmt->setString(9, layer->getTargetIpAddr().toString().c_str());
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        arp_id = res->getInt(1);
    }
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "arp_id";
    values += "?";
}

void DataBaseController::buildIcmpQuery(CapturedPackets &packet, std::shared_ptr<pcpp::IcmpLayer> &layer,
    bool &firstField, int &icmp_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::IcmpLayer>(*packet.packet.getLayerOfType<pcpp::IcmpLayer>());
    std::string query = R"(INSERT INTO icmp(icmp_type, icmp_code, icmp_checksum) VALUES (?,?,?))";
    prep_stmt = connection->prepareStatement(query);

    prep_stmt->setInt(1, static_cast<int>(layer->getIcmpHeader()->type));
    prep_stmt->setInt(2, static_cast<int>(layer->getIcmpHeader()->code));
    prep_stmt->setInt(3, static_cast<int>(layer->getIcmpHeader()->checksum));

    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        icmp_id = res->getInt(1);
    }
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "icmp_id";
    values += "?";
}

void DataBaseController::buildTcpQuery(CapturedPackets &packet, std::shared_ptr<pcpp::TcpLayer> &layer,
    bool &firstField, int &tcp_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::TcpLayer>(*packet.packet.getLayerOfType<pcpp::TcpLayer>());
    std::string query = R"(INSERT INTO tcp(tcp_src_port, tcp_dst_port, tcp_sequence_number
               , tcp_acknowledgment_number, tcp_data_offset, tcp_reserved, tcp_SYN, tcp_ACK, tcp_FIN, tcp_RST, tcp_URG, tcp_PSH,
               tcp_window, tcp_checksum, tcp_urgent_pointer, tcp_options) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?))";
    prep_stmt = connection->prepareStatement(query);
    // pcpp::TcpOptionType tcpOption;
    // auto tcp_option = layer->getTcpOption(tcpOption);
    prep_stmt->setInt(1, layer->getSrcPort());
    prep_stmt->setInt(2, layer->getDstPort());
    prep_stmt->setInt(3, layer->getTcpHeader()->sequenceNumber);
    prep_stmt->setInt(4, layer->getTcpHeader()->ackNumber);
    prep_stmt->setInt(5, layer->getTcpHeader()->dataOffset);
    prep_stmt->setInt(6, layer->getTcpHeader()->reserved);
    prep_stmt->setBoolean(7, layer->getTcpHeader()->synFlag == 1 ? true : false);
    prep_stmt->setBoolean(8, layer->getTcpHeader()->ackFlag == 1 ? true : false);
    prep_stmt->setBoolean(9, layer->getTcpHeader()->finFlag == 1 ? true : false);
    prep_stmt->setBoolean(10, layer->getTcpHeader()->rstFlag == 1 ? true : false);
    prep_stmt->setBoolean(11, layer->getTcpHeader()->urgFlag == 1 ? true : false);
    prep_stmt->setBoolean(12, layer->getTcpHeader()->pshFlag == 1 ? true : false);
    prep_stmt->setInt(13, layer->getTcpHeader()->windowSize);
    prep_stmt->setInt(14, layer->getTcpHeader()->headerChecksum);
    prep_stmt->setInt(15, layer->getTcpHeader()->urgentPointer);
    prep_stmt->setString(16, "NONE");
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        tcp_id = res->getInt(1);
    }
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "tcp_id";
    values += "?";
}

void DataBaseController::buildUdpQuery(CapturedPackets &packet, std::shared_ptr<pcpp::UdpLayer> &layer,
    bool &firstField, int &udp_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::UdpLayer>(*packet.packet.getLayerOfType<pcpp::UdpLayer>());
    std::string query = R"(INSERT INTO udp(udp_src_port, udp_dst_port, udp_length, udp_checksum) VALUES (?,?,?,?))";
    prep_stmt = connection->prepareStatement(query);

    prep_stmt->setInt(1, layer->getSrcPort());
    prep_stmt->setInt(2, layer->getDstPort());
    prep_stmt->setInt(3, layer->getDataLen());
    prep_stmt->setInt(4, layer->getUdpHeader()->headerChecksum);
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        udp_id = res->getInt(1);
    }
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "udp_id";
    values += "?";
}

void DataBaseController::buildEthernetQuery(CapturedPackets &packet, std::shared_ptr<pcpp::EthLayer> &layer,
    bool &firstField, int &ethernet_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection_local, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::EthLayer>(*packet.packet.getLayerOfType<pcpp::EthLayer>());
    std::string query = "INSERT INTO ethernet(ethernet_src_mac, ethernet_dst_mac, ethernet_eht_type) VALUES (?,?,?)";
    if (connection_local == nullptr) {
        return;
    }
    if (connection_local->isClosed()) {
        return;
    }
    if (connection_local->isValid()) {
        return;
    }
    prep_stmt = connection_local->prepareStatement(query);
    prep_stmt->setString(1, layer->getSourceMac().toString().c_str());
    prep_stmt->setString(2, layer->getDestMac().toString().c_str());
    prep_stmt->setString(3, std::to_string(layer->getEthHeader()->etherType).c_str());
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection_local->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        ethernet_id = res->getInt(1);
    }
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "ethernet_id";
    values += "?";
}

// void DataBaseController::buildSshQuery(CapturedPackets &packet, std::shared_ptr<pcpp::SSHLayer> &layer, bool firstField, int &ssh_id, std::string &packetsQuery, std::string &values) {
//     layer = std::make_shared<pcpp::SSHLayer>(*packet.packet.getLayerOfType<pcpp::SSHLayer>());
//     std::string query = R"(INSERT INTO ssh(ssh_payload) VALUES (?,?))";
//     this->prep_stmt = this->connection->prepareStatement(query);
//
//     this->prep_stmt->setString(1, layer->toString()); // assuming SSH payload is in string format
//
//     this->prep_stmt->executeUpdate();
//     query = "";
//     this->stmt = this->connection->createStatement();
//     this->res = this->stmt->executeQuery("SELECT LAST_INSERT_ID()");
//     if (this->res->next()) {
//         ssh_id = res->getInt(1);
//     }
//     if (firstField) {
//         firstField = false;
//     } else {
//         packetsQuery += ", ";
//         values += ", ";
//     }
//     packetsQuery += "ssh_id";
//     values += "?";
// }

// void DataBaseController::buildSmtpQuery(CapturedPackets &packet, std::shared_ptr<pcpp::SmtpLayer> &layer, bool firstField, int &smtp_id, std::string &packetsQuery, std::string &values) {
//     layer = std::make_shared<pcpp::SmtpLayer>(*packet.packet.getLayerOfType<pcpp::SmtpLayer>());
//     std::string query = R"(INSERT INTO smtp(smtp_payload) VALUES (?))";
//     this->prep_stmt = this->connection->prepareStatement(query);
//
//     this->prep_stmt->setString(1, layer->toString()); // assuming SMTP payload is in string format
//
//     this->prep_stmt->executeUpdate();
//     query = "";
//     this->stmt = this->connection->createStatement();
//     this->res = this->stmt->executeQuery("SELECT LAST_INSERT_ID()");
//     if (this->res->next()) {
//         smtp_id = res->getInt(1);
//     }
//     if (firstField) {
//         firstField = false;
//     } else {
//         packetsQuery += ", ";
//         values += ", ";
//     }
//     packetsQuery += "smtp_id";
//     values += "?";
// }
void DataBaseController::buildTelnetQuery(CapturedPackets &packet, std::shared_ptr<pcpp::TelnetLayer> &layer,
    bool &firstField, int &telnet_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::TelnetLayer>(*packet.packet.getLayerOfType<pcpp::TelnetLayer>());
    std::string query = R"(INSERT INTO telnet(telnet_payload) VALUES (?))";
    prep_stmt = connection->prepareStatement(query);

    prep_stmt->setString(1, layer->toString()); // assuming Telnet payload is in string format

    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        telnet_id = res->getInt(1);
    }
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "telnet_id";
    values += "?";
}

void DataBaseController::buildDhcpv4Query(CapturedPackets &packet, std::shared_ptr<pcpp::DhcpLayer> &layer,
    bool &firstField, int &dhcpv4_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::DhcpLayer>(*packet.packet.getLayerOfType<pcpp::DhcpLayer>());
    std::string query = R"(INSERT INTO dhcpv4 (op_code, hardware_type, client_ip, your_ip, server_ip, gateway_ip, client_mac, options) VALUES (?,?,?,?,?,?,?,?))";
    prep_stmt = connection->prepareStatement(query);

    // Setting basic DHCP fields
    prep_stmt->setInt(1, layer->getOpCode());
    prep_stmt->setInt(2, layer->getDhcpHeader()->opCode);
    prep_stmt->setString(3, layer->getClientIpAddress().toString().c_str());
    prep_stmt->setString(4, layer->getYourIpAddress().toString().c_str());
    prep_stmt->setString(5, layer->getServerIpAddress().toString().c_str());
    prep_stmt->setString(6, layer->getGatewayIpAddress().toString().c_str());
    prep_stmt->setString(7, layer->getClientHardwareAddress().toString().c_str());

    std::string dhcpOptions;
    auto dhcpPrevOption = layer->getFirstOptionData();
    for (int i = 0; i < layer->getOptionsCount(); i++) {
        dhcpOptions += dhcpPrevOption.getValueAsString();
        dhcpPrevOption = layer->getNextOptionData(dhcpPrevOption);
        if (dhcpPrevOption.isNull()) {
            return;
        }
    }

    std::istringstream dhcpOptionsStream(dhcpOptions.c_str());
    prep_stmt->setBlob(8, &dhcpOptionsStream);

    // Executing statement and retrieving ID
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        dhcpv4_id = res->getInt(1);
    }

    // Updating packetsQuery and values
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "dhcp_id";
    values += "?";
}

void DataBaseController::buildHttpResponseQuery(CapturedPackets &packet, std::shared_ptr<pcpp::HttpResponseLayer> &layer,
    bool &firstField, int &httpResponse_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::HttpResponseLayer>(*packet.packet.getLayerOfType<pcpp::HttpResponseLayer>());
    std::string query = R"(INSERT INTO http_responses(http_status_code, http_reason_phrase, http_version, http_headers, http_body) VALUES (?,?,?,?,?))";
    prep_stmt = connection->prepareStatement(query);

    // Setting status code and reason phrase
    prep_stmt->setInt(1, static_cast<int>(layer->getFirstLine()->getStatusCode()));
    prep_stmt->setString(2, layer->getFirstLine()->getStatusCodeString().c_str());

    // Getting and setting HTTP version
    auto version = layer->getFirstLine()->getVersion();
    prep_stmt->setString(3, std::to_string(version));

    // Serialize headers
    std::stringstream headersStream;
    for (auto field = layer->getFirstField(); field != nullptr; field = layer->getNextField(field)) {
        headersStream << field->getFieldName() << ": " << field->getFieldValue() << "\r\n";
    }
    std::string headers = headersStream.str();
    std::istringstream headersIss(headers);
    prep_stmt->setBlob(4, &headersIss);

    // Serialize body
    if (layer->getLayerPayloadSize() > 0) {
        std::string body(reinterpret_cast<const char*>(layer->getLayerPayload()), layer->getLayerPayloadSize());
        std::istringstream bodyIss(body);
        prep_stmt->setBlob(5, &bodyIss);
    } else {
        prep_stmt->setNull(5, 0);
    }

    // Executing statement and retrieving ID
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        httpResponse_id = res->getInt(1);
    }
    // Updating packetsQuery and values
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "http_response_id";
    values += "?";
}

void DataBaseController::buildHttpRequestQuery(CapturedPackets &packet, std::shared_ptr<pcpp::HttpRequestLayer> &layer,
    bool &firstField, int &httpRequest_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
   layer = std::make_shared<pcpp::HttpRequestLayer>(*packet.packet.getLayerOfType<pcpp::HttpRequestLayer>());
    std::string query = R"(INSERT INTO http_requests(http_url, http_version, http_method, http_headers, http_body) VALUES (?,?,?,?,?))";
    prep_stmt = connection->prepareStatement(query);

    // Setting URL
    prep_stmt->setString(1, layer->getUrl().c_str());

    // Getting and setting HTTP version
    auto version = layer->getFirstLine()->getVersion();
    prep_stmt->setString(2, std::to_string(layer->getFirstLine()->getVersion()));

    // Getting and setting HTTP method
    auto method = layer->getFirstLine()->getMethod();
    std::string methodString;
    switch(method) {
        case pcpp::HttpRequestLayer::HttpGET: methodString = "GET"; break;
        case pcpp::HttpRequestLayer::HttpHEAD: methodString = "HEAD"; break;
        case pcpp::HttpRequestLayer::HttpPOST: methodString = "POST"; break;
        case pcpp::HttpRequestLayer::HttpPUT: methodString = "PUT"; break;
        case pcpp::HttpRequestLayer::HttpDELETE: methodString = "DELETE"; break;
        case pcpp::HttpRequestLayer::HttpTRACE: methodString = "TRACE"; break;
        case pcpp::HttpRequestLayer::HttpOPTIONS: methodString = "OPTIONS"; break;
        case pcpp::HttpRequestLayer::HttpCONNECT: methodString = "CONNECT"; break;
        case pcpp::HttpRequestLayer::HttpPATCH: methodString = "PATCH"; break;
        default: methodString = "UNKNOWN"; break;
    }
    prep_stmt->setString(3, methodString.c_str());

    // Serialize headers
    std::stringstream headersStream;
    for (auto field = layer->getFirstField(); field != nullptr; field = layer->getNextField(field)) {
        headersStream << field->getFieldName() << ": " << field->getFieldValue() << "\r\n";
    }
    std::string headers = headersStream.str();
    std::istringstream headersIss(headers);
    prep_stmt->setBlob(4, &headersIss);

    // Serialize body
    if (layer->getLayerPayloadSize() > 0) {
        std::string body(reinterpret_cast<const char*>(layer->getLayerPayload()), layer->getLayerPayloadSize());
        std::istringstream bodyIss(body);
        prep_stmt->setBlob(5, &bodyIss);
    } else {
        prep_stmt->setNull(5, 0);
    }
    // Executing statement and retrieving ID
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        httpRequest_id = res->getInt(1);
    }
    // Updating packetsQuery and values
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "http_request_id";
    values += "?";
}

void DataBaseController::buildDnsQuery(CapturedPackets &packet, std::shared_ptr<pcpp::DnsLayer> &layer,
    bool &firstField, int &dns_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::DnsLayer>(*packet.packet.getLayerOfType<pcpp::DnsLayer>());
    std::string query = R"(INSERT INTO dns(dns_transaction_id, dns_flags, dns_questions, dns_answer_rrs,
                                            dns_authority_rrs, dns_additional_rrs, dns_queries, dns_answers) VALUES (?,?,?,?,?,?,?,?))";
    prep_stmt = connection->prepareStatement(query);
    pcpp::dnshdr * dnsHeader = layer->getDnsHeader();
    std::stringstream flags;
    flags << "QR:" << dnsHeader->queryOrResponse << ", "
          << "Opcode:" << dnsHeader->opcode << ", "
          << "AA:" << dnsHeader->authoritativeAnswer << ", "
          << "TC:" << dnsHeader->truncation << ", "
          << "RD:" << dnsHeader->recursionDesired << ", "
          << "RA:" << dnsHeader->recursionAvailable << ", "
          << "Z:" << dnsHeader->zero << ", "
          << "AD:" << dnsHeader->authenticData << ", "
          << "CD:" << dnsHeader->checkingDisabled << ", "
          << "RCODE:" << dnsHeader->responseCode;
    prep_stmt->setInt(1, layer->getDnsHeader()->transactionID);
    prep_stmt->setString(2, flags.str().c_str());
    prep_stmt->setInt(3, layer->getQueryCount());
    prep_stmt->setInt(4, layer->getAnswerCount());
    prep_stmt->setInt(5, layer->getAuthorityCount());
    prep_stmt->setInt(6, layer->getAdditionalRecordCount());
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        dns_id = res->getInt(1);
    }
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "dns_id";
    values += "?";
}

void DataBaseController::buildFtpRequestQuery(CapturedPackets &packet, std::shared_ptr<pcpp::FtpRequestLayer> &layer,
    bool &firstField, int &ftpRequest_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    layer = std::make_shared<pcpp::FtpRequestLayer>(*packet.packet.getLayerOfType<pcpp::FtpRequestLayer>());
    std::string query = R"(INSERT INTO ftp_requests(ftp_command, ftp_arguments, ftp_headers, ftp_payload) VALUES (?,?,?,?))";
    prep_stmt = connection->prepareStatement(query);

    // Setting FTP command
    std::string ftpCommand = pcpp::FtpRequestLayer::getCommandAsString(layer->getCommand());
    prep_stmt->setString(1, ftpCommand.c_str());

    // Setting FTP arguments (command option)
    std::string ftpArguments = layer->getCommandOption();
    prep_stmt->setString(2, ftpArguments.c_str());

    // Serialize headers (if any)
    std::stringstream headersStream;
    // Add code to retrieve headers if required. FTP typically does not have headers like HTTP.
    std::string headers = headersStream.str();
    std::istringstream headersIss(headers);
    prep_stmt->setBlob(3, &headersIss);

    // Serialize payload
    if (layer->getLayerPayloadSize() > 0) {
        std::string payload(reinterpret_cast<const char*>(layer->getLayerPayload()), layer->getLayerPayloadSize());
        std::istringstream payloadIss(payload);
        prep_stmt->setBlob(4, &payloadIss);
    } else {
        prep_stmt->setNull(4, 0);
    }

    // Executing statement and retrieving ID
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        ftpRequest_id = res->getInt(1);
    }
    // Updating packetsQuery and values
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "ftp_request_id";
    values += "?";
}

void DataBaseController::buildFtpResponseQuery(CapturedPackets &packet, std::shared_ptr<pcpp::FtpResponseLayer> &layer,
    bool &firstField, int &ftpResponse_id, std::string &packetsQuery, std::string &values, sql::Connection* &connection, sql::PreparedStatement* &prep_stmt, sql::Statement* &stmt, sql::ResultSet* &res) {
    // Initialize the ftpResponseLayer with data from the current packet
    layer = std::make_shared<pcpp::FtpResponseLayer>(*packet.packet.getLayerOfType<pcpp::FtpResponseLayer>());
    std::string query = R"(INSERT INTO ftp_responses(ftp_status_code, ftp_status_string, ftp_headers, ftp_payload) VALUES (?,?,?,?))";
    prep_stmt = connection->prepareStatement(query);

    // Set FTP status code
    int statusCode = static_cast<int>(layer->getStatusCode());
    prep_stmt->setInt(1, statusCode);

    // Set FTP status string
    std::string statusString = pcpp::FtpResponseLayer::getStatusCodeAsString(layer->getStatusCode());
    prep_stmt->setString(2, statusString);

    // Serialize headers (if any)
    std::stringstream headersStream;
    // Add code to retrieve headers if required. FTP typically does not have headers like HTTP.
    std::string headers = headersStream.str();
    std::istringstream headersIss(headers);
    prep_stmt->setBlob(3, &headersIss);

    // Serialize payload
    if (layer->getLayerPayloadSize() > 0) {
        std::string payload(reinterpret_cast<const char*>(layer->getLayerPayload()), layer->getLayerPayloadSize());
        std::istringstream payloadIss(payload);
        prep_stmt->setBlob(4, &payloadIss);
    } else {
        prep_stmt->setNull(4, 0);
    }

    // Executing statement and retrieving ID
    prep_stmt->executeUpdate();
    query = "";
    stmt = connection->createStatement();
    res = stmt->executeQuery("SELECT LAST_INSERT_ID()");
    if (res->next()) {
        ftpResponse_id = res->getInt(1);
    }

    // Updating packetsQuery and values
    if (firstField) {
        firstField = false;
    } else {
        packetsQuery += ", ";
        values += ", ";
    }
    packetsQuery += "ftp_response_id";
    values += "?";
}
