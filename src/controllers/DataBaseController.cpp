//
// Created by tomasz on 02.10.24.
//

#include "controllers/DataBaseController.h"
#include "controllers/LogController.h"
#include "utils/Utils.h"
#include "global/Global.h"

#include "Packet.h"
#include "IPv4Layer.h"
#include "IPv6Layer.h"
#include "EthLayer.h"
#include "ArpLayer.h"
#include "DnsLayer.h"
#include "IcmpLayer.h"
#include "IcmpV6Layer.h"
#include "FtpLayer.h"
#include "SmtpLayer.h"
#include "IgmpLayer.h"
#include "TcpLayer.h"
#include "UdpLayer.h"

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
    this->dbThread = std::make_shared<std::thread>(&DataBaseController::dataBaseThread, this);
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

void DataBaseController::setMainController(std::shared_ptr<MainController> mainController) {
    this->mainController = mainController;
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

void DataBaseController::dataBaseThread() {
    LogController::getInstance()->addLog(Utils::getTime(), "Database thread", LogType::SUCCESFULL);
    while (1) {
        if (!this->mainController->getCapturedPacketVectorDatabase().empty()) {
            std::lock_guard<std::mutex> lock(guard_4);
            CapturedPackets packet = this->mainController->getCapturedPacketVectorDatabase().back();
            this->mainController->getCapturedPacketVectorDatabase().pop_back();
            insertNewPacket(packet);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}


void DataBaseController::insertNewPacket(CapturedPackets &packet) {
    if (mainController == nullptr) {
        LogController::getInstance()->addLog(Utils::getTime(), "Cannot insert new packet mainController is null", LogType::ERROR);
        return;
    }
    int ipv4_id{-1}, ipv6_id{-1}, arp_id{-1}, icmp_id{-1}, index{2};
    int icmpv6_id{-1}, igmp_id{-1}, tcp_id{-1}, udp_id{-1}, ethernet_id{-1};
    std::shared_ptr<pcpp::IPv4Layer> ipv4Layer;
    std::shared_ptr<pcpp::IPv6Layer> ipv6Layer;
    std::shared_ptr<pcpp::ArpLayer> arpLayer;
    std::shared_ptr<pcpp::IcmpLayer> icmpLayer;
    std::shared_ptr<pcpp::IcmpV6Layer> icmpV6Layer;
    std::shared_ptr<pcpp::IgmpLayer> igmpLayer;
    std::shared_ptr<pcpp::TcpLayer> tcpLayer;
    std::shared_ptr<pcpp::UdpLayer> udpLayer;
    std::shared_ptr<pcpp::EthLayer> ethLayer;
    std::string query = "";
    bool firstField = true;
    std::string packetsQuery = "INSERT INTO packets(packet_id, ";
    std::string values = "VALUES(?, ";
    try {
        if (packet.packet.isPacketOfType(pcpp::Ethernet)) {
            ethLayer = std::make_shared<pcpp::EthLayer>(*packet.packet.getLayerOfType<pcpp::EthLayer>());
            query = "INSERT INTO ethernet(ethernet_src_mac, ethernet_dst_mac, ethernet_eht_type) VALUES (?,?,?)";
            this->prep_stmt = this->connection->prepareStatement(query);
            this->prep_stmt->setString(1, ethLayer->getSourceMac().toString().c_str());
            this->prep_stmt->setString(2, ethLayer->getDestMac().toString().c_str());
            this->prep_stmt->setString(3, std::to_string(ethLayer->getEthHeader()->etherType).c_str());
            this->prep_stmt->executeUpdate();
            query = "";
            this->stmt = this->connection->createStatement();
            this->res = this->stmt->executeQuery("SELECT LAST_INSERT_ID()");
            if (this->res->next()) {
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
        if (packet.packet.isPacketOfType(pcpp::IPv4)) {
            ipv4Layer = std::make_shared<pcpp::IPv4Layer>(*packet.packet.getLayerOfType<pcpp::IPv4Layer>());
            query = R"(INSERT INTO ipv4(ipv4_version, ipv4_ihl, ipv4_tos, ipv4_total_length,
                    ipv4_identification, ipv4_flags, ipv4_fragment_offset, ipv4_ttl, ipv4_protocol,
                    ipv4_header_checksum, ipv4_src_ip, ipv4_dst_ip) VALUES (?,?,?,?,?,?,?,?,?,?,?,?))";
            std::cout << query << std::endl;
            this->prep_stmt = this->connection->prepareStatement(query);
            this->prep_stmt->setInt(1, ipv4Layer->getIPv4Header()->ipVersion);
            this->prep_stmt->setInt(2, ipv4Layer->getIPv4Header()->internetHeaderLength);
            this->prep_stmt->setInt(3, ipv4Layer->getIPv4Header()->typeOfService);
            this->prep_stmt->setInt(4, ipv4Layer->getIPv4Header()->totalLength);
            this->prep_stmt->setInt(5, ipv4Layer->getIPv4Header()->ipId);
            this->prep_stmt->setInt(6, ipv4Layer->getFragmentFlags());
            this->prep_stmt->setInt(7, ipv4Layer->getIPv4Header()->fragmentOffset);
            this->prep_stmt->setInt(8, ipv4Layer->getIPv4Header()->timeToLive);
            this->prep_stmt->setInt(9, ipv4Layer->getIPv4Header()->protocol);
            this->prep_stmt->setInt(10, ipv4Layer->getIPv4Header()->headerChecksum);
            this->prep_stmt->setString(11, ipv4Layer->getSrcIPAddress().toString().c_str());
            this->prep_stmt->setString(12, ipv4Layer->getDstIPAddress().toString().c_str());
            std::cout << query << std::endl;
            this->prep_stmt->executeUpdate();
            query = "";
            this->stmt = this->connection->createStatement();
            this->res = this->stmt->executeQuery("SELECT LAST_INSERT_ID()");
            if (this->res->next()) {
                ipv4_id = res->getInt(1);
            }
            if (firstField) {
                firstField = false;
            } else {
                packetsQuery += ", ";
                values += ", ";
            }
            packetsQuery += "ivp4_id";
            values += "?";
        }
        if (packet.packet.isPacketOfType(pcpp::IPv6)) {
            ipv6Layer = std::make_shared<pcpp::IPv6Layer>(*packet.packet.getLayerOfType<pcpp::IPv6Layer>());
            query = R"(INSERT INTO ipv6(ipv6_version, ipv6_traffic_class, ipv6_flow_label, ipv6_payload_length,
            ipv6_next_header, ipv6_hop_limit, ipv6_src_ip, ipv6_dst_ip) VALUES (?,?,?,?,?,?,?,?))";
            this->prep_stmt = this->connection->prepareStatement(query);
            this->prep_stmt->setInt(1, ipv6Layer->getIPv6Header()->ipVersion);
            this->prep_stmt->setInt(2, ipv6Layer->getIPv6Header()->trafficClass);
            int combined = (static_cast<int>(ipv6Layer->getIPv6Header()->flowLabel[0]) << 16) |
                           (static_cast<int>(ipv6Layer->getIPv6Header()->flowLabel[1]) << 8) |
                            static_cast<int>(ipv6Layer->getIPv6Header()->flowLabel[2]);
            this->prep_stmt->setInt(3, combined);
            this->prep_stmt->setInt(4, ipv6Layer->getIPv6Header()->payloadLength);
            this->prep_stmt->setInt(5, ipv6Layer->getIPv6Header()->nextHeader);
            this->prep_stmt->setInt(6, ipv6Layer->getIPv6Header()->hopLimit);
            this->prep_stmt->setString(7, ipv6Layer->getSrcIPAddress().toString().c_str());
            this->prep_stmt->setString(8, ipv6Layer->getDstIPAddress().toString().c_str());
            this->prep_stmt->executeUpdate();
            query = "";
            this->stmt = this->connection->createStatement();
            this->res = this->stmt->executeQuery("SELECT LAST_INSERT_ID()");
            if (this->res->next()) {
                ipv6_id = res->getInt(1);
            }
            if (firstField) {
                firstField = false;
            } else {
                packetsQuery += ", ";
                values += ", ";
            }
            packetsQuery += "ivp4_id";
            values += "?";
        }
        if (packet.packet.isPacketOfType(pcpp::ARP)) {
            arpLayer = std::make_shared<pcpp::ArpLayer>(*packet.packet.getLayerOfType<pcpp::ArpLayer>());
            query = R"(INSERT INTO arp(arp_hw_type, arp_protocol_type, arp_hw_size, arp_protocol_size,
            arp_opcode, arp_src_hw_addr, arp_src_protocol_addr, arp_dst_hw_addr, arp_dst_protocol_addr) VALUES (?,?,?,?,?,?,?,?,?))";
            this->prep_stmt = this->connection->prepareStatement(query);
            this->prep_stmt->setInt(1, static_cast<int>(arpLayer->getArpHeader()->hardwareType));
            this->prep_stmt->setInt(2, static_cast<int>(arpLayer->getArpHeader()->protocolType));
            this->prep_stmt->setInt(3, static_cast<int>(arpLayer->getArpHeader()->hardwareSize));
            this->prep_stmt->setInt(4, static_cast<int>(arpLayer->getArpHeader()->protocolSize));
            this->prep_stmt->setInt(5, static_cast<int>(arpLayer->getArpHeader()->opcode));
            this->prep_stmt->setString(6, arpLayer->getSenderMacAddress().toString().c_str());
            this->prep_stmt->setString(7, arpLayer->getSenderIpAddr().toString().c_str());
            this->prep_stmt->setString(8, arpLayer->getTargetMacAddress().toString().c_str());
            this->prep_stmt->setString(9, arpLayer->getTargetIpAddr().toString().c_str());
            this->prep_stmt->executeUpdate();
            query = "";
            this->stmt = this->connection->createStatement();
            this->res = this->stmt->executeQuery("SELECT LAST_INSERT_ID()");
            if (this->res->next()) {
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
        // if (packet.packet.isPacketOfType(pcpp::ICMP)) {
        //     icmpLayer = std::make_shared<pcpp::IcmpLayer>(*packet.packet.getLayerOfType<pcpp::IcmpLayer>());
        //     query = R"(INSERT INTO icmp(icmpv4_type, icmpv4_code, icmpv4_checksum, icmpv4_rest_of_header) VALUES (?,?,?,?))";
        //     this->prep_stmt = this->connection->prepareStatement(query);
        //     this->prep_stmt->setInt(1, static_cast<int>(icmpLayer->getIcmpHeader()->type));
        //     this->prep_stmt->setInt(2, static_cast<int>(icmpLayer->getIcmpHeader()->code));
        //     this->prep_stmt->setInt(3, static_cast<int>(icmpLayer->getIcmpHeader()->checksum));
        //     this->prep_stmt->setBlob(icmpLayer->get)
        //     this->prep_stmt->executeUpdate();
        //     query = "";
        // }
        if (packet.packet.isPacketOfType(pcpp::ICMPv6)) {
            icmpV6Layer = std::make_shared<pcpp::IcmpV6Layer>(*packet.packet.getLayerOfType<pcpp::IcmpV6Layer>());
        }
        if (packet.packet.isPacketOfType(pcpp::TCP)) {
            tcpLayer = std::make_shared<pcpp::TcpLayer>(*packet.packet.getLayerOfType<pcpp::TcpLayer>());
            query = R"(INSERT INTO tcp(tcp_src_port, tcp_dst_port, tcp_sequence_number
                    , tcp_acknowledgment_number, tcp_data_offset, tcp_reserved, tcp_SYN, tcp_ACK, tcp_FIN, tcp_RST, tcp_URG, tcp_PSH,
                    tcp_window, tcp_checksum, tcp_urgent_pointer, tcp_options) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?))";
            this->prep_stmt = this->connection->prepareStatement(query);

            this->prep_stmt->setInt(1, tcpLayer->getSrcPort());
            this->prep_stmt->setInt(2, tcpLayer->getDstPort());
            this->prep_stmt->setUInt(3, tcpLayer->getTcpHeader()->sequenceNumber);
            this->prep_stmt->setUInt(4, tcpLayer->getTcpHeader()->ackNumber);
            this->prep_stmt->setInt(5, tcpLayer->getTcpHeader()->dataOffset);
            this->prep_stmt->setInt(6, tcpLayer->getTcpHeader()->reserved);
            this->prep_stmt->setBoolean(7, tcpLayer->getTcpHeader()->synFlag == 1 ? true : false);
            this->prep_stmt->setBoolean(8, tcpLayer->getTcpHeader()->ackFlag == 1 ? true : false);
            this->prep_stmt->setBoolean(9, tcpLayer->getTcpHeader()->finFlag == 1 ? true : false);
            this->prep_stmt->setBoolean(10, tcpLayer->getTcpHeader()->rstFlag == 1 ? true : false);
            this->prep_stmt->setBoolean(11, tcpLayer->getTcpHeader()->urgFlag == 1 ? true : false);
            this->prep_stmt->setBoolean(12, tcpLayer->getTcpHeader()->pshFlag == 1 ? true : false);
            this->prep_stmt->setInt(13, tcpLayer->getTcpHeader()->windowSize);
            this->prep_stmt->setInt(14, tcpLayer->getTcpHeader()->headerChecksum);
            this->prep_stmt->setInt(15, tcpLayer->getTcpHeader()->urgentPointer);
            this->prep_stmt->executeUpdate();
            query = "";
            this->stmt = this->connection->createStatement();
            this->res = this->stmt->executeQuery("SELECT LAST_INSERT_ID()");
            if (this->res->next()) {
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
        if (packet.packet.isPacketOfType(pcpp::UDP)) {
            udpLayer = std::make_shared<pcpp::UdpLayer>(*packet.packet.getLayerOfType<pcpp::UdpLayer>());
            query = R"(INSERT INTO udp(udp_src_port, udp_dst_port, udp_length, udp_checksum) VALUES (?,?,?,?))";
            this->prep_stmt = this->connection->prepareStatement(query);

            this->prep_stmt->setInt(1, udpLayer->getSrcPort());
            this->prep_stmt->setInt(2, udpLayer->getDstPort());
            this->prep_stmt->setInt(3, udpLayer->getDataLen());
            this->prep_stmt->setInt(4, udpLayer->getUdpHeader()->headerChecksum);
            this->prep_stmt->executeUpdate();
            query = "";
            this->stmt = this->connection->createStatement();
            this->res = this->stmt->executeQuery("SELECT LAST_INSERT_ID()");
            if (this->res->next()) {
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
        if (packet.packet.isPacketOfType(pcpp::HTTP)) {

        }
        if (packet.packet.isPacketOfType(pcpp::FTP)) {

        }
        if (packet.packet.isPacketOfType(pcpp::SSH)) {

        }
        if (packet.packet.isPacketOfType(pcpp::Telnet)) {

        }
        if (packet.packet.isPacketOfType(pcpp::DNS)) {

        }
        if (packet.packet.isPacketOfType(pcpp::DHCP)) {

        }
        if (packet.packet.isPacketOfType(pcpp::DHCPv6)) {

        }
        if (packet.packet.isPacketOfType(pcpp::IGMP)) {

        }
        if (packet.packet.isPacketOfType(pcpp::IGMPv1)) {

        }
        if (packet.packet.isPacketOfType(pcpp::IGMPv2)) {

        }
        if (packet.packet.isPacketOfType(pcpp::IGMPv3)) {

        }
        int ipv4_id{-1}, ipv6_id{-1}, arp_id{-1}, icmp_id{-1}, icmpv6_id{-1}, igmp_id{-1}, tcp_id{-1}, udp_id{-1}, ethernet_id{-1};
        query.clear();
        query = packetsQuery + values;
        this->prep_stmt->setInt(1, packet.id);
        this->prep_stmt = this->connection->prepareStatement(query);
        if (ethernet_id != -1) {
            this->prep_stmt->setInt(index++, ethernet_id);
        }
        if (ipv4_id != -1) {
            this->prep_stmt->setInt(index++, ipv4_id);
        }
        if (ipv6_id != -1) {
            this->prep_stmt->setInt(index++, ipv6_id);
        }
        if (arp_id != -1) {
            this->prep_stmt->setInt(index++, arp_id);
        }
        if (icmp_id != -1) {
            this->prep_stmt->setInt(index++, icmp_id);
        }
        if (icmpv6_id != -1) {
            this->prep_stmt->setInt(index++, icmpv6_id);
        }
        if (tcp_id != -1) {
            this->prep_stmt->setInt(index++, tcp_id);
        }
        if (udp_id != -1) {
            this->prep_stmt->setInt(index++, udp_id);
        }
        this->prep_stmt->executeUpdate();

    } catch (sql::SQLException &e) {
        LogController::getInstance()->addLog(Utils::getTime(), e.what() + std::string(" ") + __PRETTY_FUNCTION__, LogType::WARNING);
    }
}



