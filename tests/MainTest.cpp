//
// Created by tomasz on 12/31/24.
//

#include <gtest/gtest.h>
#include "controllers/MainController.h"
#include "views/PacketCaptureView.h"
#include "models/PacketCaptureModel.h"
#include "listeners/Listener.h"
#include "utils/Utils.h"
#include "controllers/FilterController.h"
#include "models/FilterModel.h"
#include "views/FilterView.h"
#include "controllers/DeviceController.h"
#include "models/DeviceModel.h"
#include <views/DeviceView.h>
#include <views/PacketView.h>
#include "controllers/StatisticController.h"
#include "models/StatisticModel.h"
#include "views/StatisticView.h"
#include "controllers/LogController.h"
#include "models/LogModel.h"
#include "views/LogView.h"
#include "controllers/DataBaseController.h"
#include "models/DataBaseModel.h"
#include "views/DataBaseView.h"
#include "controllers/AnalyzerController.h"
#include "models/AnalyzerModel.h"
#include "views/AnalyzerView.h"
#include "controllers/SearchController.h"
#include "models/SearchModel.h"
#include "views/SearchView.h"

TEST(MainTest, InsertingPacket) {
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
    auto statisticView = std::make_shared<StatisticView>();
    auto statisticModel = std::make_shared<StatisticModel>("20:20:20");

    auto packetCaptureView = std::make_shared<PacketCaptureView>();
    auto packetCaptureModel = std::make_shared<PacketCaptureModel>();
    auto packetListener = std::make_shared<Listener>(packetCaptureModel);
    auto statisticController = std::make_shared<StatisticController>(statisticModel, statisticView, packetListener);
    auto mainController = std::make_shared<MainController>(packetCaptureModel, packetCaptureView, packetListener, statisticController);

    packetCaptureModel->setMainController(mainController);
    pcpp::Packet packet;
    mainController->insertPacketToTable(packets[0].packet);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    EXPECT_EQ(mainController->getCapturedVectorData().size(), 1);
    mainController->clearTableOfPackets();
    EXPECT_EQ(mainController->getCapturedVectorData().size(), 0);
}