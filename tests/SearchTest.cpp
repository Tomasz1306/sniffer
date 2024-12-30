//
// Created by tomasz on 12/29/24.
//

#include <gtest/gtest.h>
#include "controllers/SearchController.h"
#include "listeners/Listener.h"
#include "models/SearchModel.h"
#include "views/SearchView.h"

TEST(SearchTest, TestFindingPacketByKey) {
    auto model = std::make_shared<SearchModel>();
    auto view = std::make_shared<SearchView>();
    auto controller = std::make_shared<SearchController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 10; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer *ethLayer =
            new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);
        std::string ip;
        if (i == 3) {
          ip = "127.0.0.2";
        } else {
          ip = "127.0.0." + std::to_string(i);
        }
        pcpp::IPv4Address srcIP(ip);
        pcpp::IPv4Address dstIP("192.168.1.2");
        pcpp::IPv4Layer *ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);

        uint16_t srcPort = 12345 + i;
        uint16_t dstPort = 22;
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
    for (int i = 0; i <= 19; i++) {
        controller->search("127.0.0.2", packets);
    }
    EXPECT_EQ(controller->getFindedPackets().size(), 2);
}