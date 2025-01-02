//
// Created by tomasz on 12/30/24.
//
#include <IcmpV6Layer.h>
#include <gtest/gtest.h>
#include "controllers/DataBaseController.h"
#include "controllers/MainController.h"
#include "models/PacketModel.h"
#include "models/PacketCaptureModel.h"
#include "models/DataBaseModel.h"
#include "views/PacketView.h"
#include "views/PacketCaptureView.h"
#include "views/DataBaseView.h"
#include "EthLayer.h"
#include "HttpLayer.h"
#include "IPv4Layer.h"
#include "Packet.h"
#include "PayloadLayer.h"
#include "PcapFileDevice.h"
#include "SystemUtils.h"
#include "TcpLayer.h"
#include "controllers/AnalyzerController.h"
#include "models/PacketModel.h"
#include "stdlib.h"
#include "gtest/gtest.h"
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include "cppconn/prepared_statement.h"
#include "IcmpV6Layer.h"

TEST(DatabaseTest, TestConnection) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->setCatchPackets(false);
    controller->initializeDbThreadPool();
    EXPECT_EQ(controller->isConnectedToDataBase(), true);
    controller->disconnectFromDataBase();
    EXPECT_EQ(controller->isConnectedToDataBase(), false);
}
TEST(DatabaseTest, TestCreatingDatabase) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(3);
    controller->setCatchPackets(false);
    controller->initializeDbThreadPool();
    controller->newDatabase("TEST_DATABASE");
    EXPECT_EQ(controller->getDatabaseCreated(), true);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, TestCreateTable) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(1);
    controller->setCatchPackets(false);
    controller->useDatabase();
    // controller->initializeDbThreadPool();
    controller->createTables();
    std::this_thread::sleep_for(std::chrono::seconds(3));
    EXPECT_EQ(controller->getTableCreated(), true);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, InsertEthernetProtocol) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 1; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer *ethLayer =
            new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        pcpp::IPv4Layer *ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);

        uint16_t srcPort = 12345;
        uint16_t dstPort = 80;
        pcpp::TcpLayer *tcpLayer = new pcpp::TcpLayer(srcPort, dstPort);
        tcpLayer->getTcpHeader()->synFlag = 1;
        tcpLayer->getTcpHeader()->windowSize = htons(65535);
        packet.addLayer(tcpLayer);

        tcpLayer->computeCalculateFields();
        ipLayer->computeCalculateFields();
        ethLayer->computeCalculateFields();
        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->ethernetCount, 1);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, InsertIPv4Protocol) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 1; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer *ethLayer =
            new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        pcpp::IPv4Layer *ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);

        uint16_t srcPort = 12345;
        uint16_t dstPort = 80;
        pcpp::TcpLayer *tcpLayer = new pcpp::TcpLayer(srcPort, dstPort);
        tcpLayer->getTcpHeader()->synFlag = 1;
        tcpLayer->getTcpHeader()->windowSize = htons(65535);
        packet.addLayer(tcpLayer);

        tcpLayer->computeCalculateFields();
        ipLayer->computeCalculateFields();
        ethLayer->computeCalculateFields();
        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->ipv4Count, 1);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, InsertIPv6Protocol) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 1; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer *ethLayer =
            new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv6Layer *ipLayer = new pcpp::IPv6Layer(pcpp::IPv6Address("fe80::5d66:5f96:e86d:5480"), pcpp::IPv6Address("2a02:a315:c3dc:f780:12ff:e0ff:fe28:3cbf"));
        packet.addLayer(ipLayer);

        uint16_t srcPort = 12345;
        uint16_t dstPort = 80;
        pcpp::TcpLayer *tcpLayer = new pcpp::TcpLayer(srcPort, dstPort);
        tcpLayer->getTcpHeader()->synFlag = 1;
        tcpLayer->getTcpHeader()->windowSize = htons(65535);
        packet.addLayer(tcpLayer);

        tcpLayer->computeCalculateFields();
        ipLayer->computeCalculateFields();
        ethLayer->computeCalculateFields();
        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->ipv6Count, 1);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, insertTcpProtocol) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 1; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer *ethLayer =
            new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        pcpp::IPv4Layer *ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);

        uint16_t srcPort = 12345;
        uint16_t dstPort = 80;
        pcpp::TcpLayer *tcpLayer = new pcpp::TcpLayer(srcPort, dstPort);
        tcpLayer->getTcpHeader()->synFlag = 1;
        tcpLayer->getTcpHeader()->windowSize = htons(65535);
        packet.addLayer(tcpLayer);
        tcpLayer->computeCalculateFields();
        ipLayer->computeCalculateFields();
        ethLayer->computeCalculateFields();
        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->tcpCount, 1);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, InsertUdpProtocol) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 1; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer *ethLayer =
            new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        pcpp::IPv4Layer *ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);

        uint16_t srcPort = 12345;
        uint16_t dstPort = 80;
        pcpp::UdpLayer *udpLayer = new pcpp::UdpLayer(srcPort, dstPort);
        packet.addLayer(udpLayer);

        ipLayer->computeCalculateFields();
        ethLayer->computeCalculateFields();
        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->udpCount, 1);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, insertArpLayer) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 1; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        pcpp::EthLayer *ethLayer =
            new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::ArpLayer *arpLayer = new pcpp::ArpLayer(pcpp::ARP_REQUEST, srcMac, dstMac, srcIP, dstIP);

        packet.addLayer(arpLayer);
        ethLayer->computeCalculateFields();
        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->arpCount, 1);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, insertIcmpProtocol) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 2; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer *ethLayer =
            new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        pcpp::IPv4Layer *ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);

        pcpp::IcmpLayer *icmpLayer = new pcpp::IcmpLayer();
        packet.addLayer(icmpLayer);

        ipLayer->computeCalculateFields();
        ethLayer->computeCalculateFields();
        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->icmpCount, 1);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, insertIcmpv6Protocol) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 2; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer *ethLayer =
            new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv6Address srcIP("2a02:a315:c3dc:f780:12ff:e0ff:fe28:3cbf");
        pcpp::IPv6Address dstIP("2a02:a315:c3dc:f780:b137:1edd:6c78:9d0e");
        pcpp::IPv6Layer *ipLayer = new pcpp::IPv6Layer(srcIP, dstIP);
        packet.addLayer(ipLayer);

        uint8_t *data = new uint8_t(34);
        pcpp::IcmpV6Layer *icmpv6Layer = new pcpp::IcmpV6Layer(pcpp::ICMPv6MessageType::ICMPv6_ECHO_REPLY, 129, data, sizeof(data));

        packet.addLayer(icmpv6Layer);

        ipLayer->computeCalculateFields();
        ethLayer->computeCalculateFields();
        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->icmpv6Count, 1);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, insertHttpRequestLayer) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 1; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer *ethLayer =
            new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        pcpp::IPv4Layer *ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);

        uint16_t srcPort = 12345;
        uint16_t dstPort = 80;
        pcpp::TcpLayer *tcpLayer = new pcpp::TcpLayer(srcPort, dstPort);
        tcpLayer->getTcpHeader()->synFlag = 1;
        tcpLayer->getTcpHeader()->windowSize = htons(65535);
        packet.addLayer(tcpLayer);
        pcpp::HttpRequestLayer *httpRequestLayer =
                 new pcpp::HttpRequestLayer(pcpp::HttpRequestLayer::HttpGET,
                                            "/index.php", pcpp::HttpVersion::OneDotOne);
        httpRequestLayer->computeCalculateFields();
        httpRequestLayer->addEndOfHeader();
        packet.addLayer(httpRequestLayer);
        std::string data = "<script>alert('XSS')</script>";
        pcpp::PayloadLayer *payloadLayer =
            new pcpp::PayloadLayer((uint8_t *)data.c_str(), data.size());
        packet.addLayer(payloadLayer);
        tcpLayer->computeCalculateFields();
        ipLayer->computeCalculateFields();
        ethLayer->computeCalculateFields();
        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }
    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->httpRequestCount, 1);
    controller->disconnectFromDataBase();
}
TEST(DatabaseTest, insertHttpResponseLayer) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 1; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        auto ethLayer = new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        auto ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);

        uint16_t srcPort = 80;
        uint16_t dstPort = 80;
        auto tcpLayer = new pcpp::TcpLayer(srcPort, dstPort);
        tcpLayer->getTcpHeader()->ackFlag = 1;  // Ustaw ACK
        packet.addLayer(tcpLayer);

        auto httpResponseLayer = new pcpp::HttpResponseLayer(
            pcpp::HttpVersion::OneDotOne,
            pcpp::HttpResponseLayer::HttpResponseStatusCode::Http200OK,
            "OK");
        httpResponseLayer->addField("Content-Type", "text/plain");
        httpResponseLayer->addField("Content-Length", "13");
        httpResponseLayer->addEndOfHeader();
        packet.addLayer(httpResponseLayer);
        packet.computeCalculateFields();

        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->httpResponseCount, 1);
    controller->disconnectFromDataBase();
}

TEST(DatabaseTest, insertFtpRequestLayer) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 1; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        auto ethLayer = new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        auto ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);

        uint16_t srcPort = 1033;
        uint16_t dstPort = 21;
        auto tcpLayer = new pcpp::TcpLayer(srcPort, dstPort);
        tcpLayer->getTcpHeader()->ackFlag = 1;
        tcpLayer->getTcpHeader()->ackNumber = 0;
        tcpLayer->getTcpHeader()->sequenceNumber = 1000;

        packet.addLayer(tcpLayer);

        pcpp::FtpRequestLayer *ftpLayer = new pcpp::FtpRequestLayer(pcpp::FtpRequestLayer::FtpCommand::DSIZ, "COMMAND");
        packet.addLayer(ftpLayer);

        packet.computeCalculateFields();

        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet), false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->ftpRequestCount, 1);
    controller->disconnectFromDataBase();
}


TEST(DatabaseTest, insertFtpResponseLayer) {
    auto view = std::make_shared<DataBaseView>();
    auto model = std::make_shared<DataBaseModel>();
    auto controller = std::make_shared<DataBaseController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 1; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        auto ethLayer = new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        auto ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);

        uint16_t srcPort = 21;
        uint16_t dstPort = 2342;
        auto tcpLayer = new pcpp::TcpLayer(srcPort, dstPort);
        tcpLayer->getTcpHeader()->ackFlag = 1;  // Ustaw ACK
        packet.addLayer(tcpLayer);

        pcpp::FtpResponseLayer *ftpLayer = new pcpp::FtpResponseLayer(pcpp::FtpResponseLayer::FtpStatusCode::COMMAND_OK, "200");
        packet.addLayer(ftpLayer);

        packet.computeCalculateFields();

        packets.emplace_back(0, false, "2024-12-15 12:00:00", std::move(packet),
                             false);
    }

    controller->connectToDataBase("localhost", "3306", "root", "root");
    controller->loadDatabases();
    controller->selectIndex(0);
    controller->setCatchPackets(false);
    controller->useDatabase();
    controller->insertNewPacket(packets[0], controller->getConnection());

    EXPECT_EQ(controller->ftpResponseCount, 1);
    controller->disconnectFromDataBase();
}

