//
// Created by tomasz on 1/1/25.
//

#include <gtest/gtest.h>
#include "controllers/StatisticController.h"
#include "models/StatisticModel.h"
#include "views/StatisticView.h"
#include "listeners/Listener.h"

TEST(StatisticTest, create) {
    auto listener = std::make_shared<Listener>();
    auto model = std::make_shared<StatisticModel>("20:20:20");
    auto view = std::make_shared<StatisticView>();
    auto controller = std::make_shared<StatisticController>(model,view,listener);

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

    controller->addPacketToStatistics(packets[0].packet);
    EXPECT_EQ(controller->getModel()->ftpCount, 1);
    EXPECT_EQ(controller->getModel()->tcpCount, 0);
    EXPECT_EQ(controller->getModel()->ipv4Count, 0);
}