//
// Created by tomasz on 12/15/24.
//
#include "gtest/gtest.h"
#include "controllers/AnalyzerController.h"
#include "controllers/DeviceController.h"
#include "models/DeviceModel.h"
#include "views/DeviceView.h"
#include "listeners/Listener.h"
#include "Packet.h"
#include "EthLayer.h"
#include "IPv4Layer.h"
#include "TcpLayer.h"
#include "HttpLayer.h"
#include "PayloadLayer.h"
#include "stdlib.h"
#include "SystemUtils.h"
#include "PcapFileDevice.h"
#include "models/PacketModel.h"

class MockCapturedPackets : public CapturedPackets {
public:
    MockCapturedPackets(const std::string& sourceIP, uint16_t destPort, const std::string& captureTime,
                   const std::string& uri = "", const std::string& payload = "")
                   : CapturedPackets(0, false, captureTime, pcpp::Packet(1024), true) {
        // Create Ethernet layer and add it to the packet
        pcpp::EthLayer* newEthernetLayer = new pcpp::EthLayer(
            pcpp::MacAddress("00:50:43:11:22:33"),
            pcpp::MacAddress("aa:bb:cc:dd:ee:ff"));
        // Add to packet ownership (heap allocated)

        // Create IPv4 layer and add it to the packet
        pcpp::IPv4Layer* newIPLayer = new pcpp::IPv4Layer(
            pcpp::IPv4Address("192.168.1.1"),
            pcpp::IPv4Address("10.0.0.1"));
        newIPLayer->getIPv4Header()->ipId = pcpp::hostToNet16(2000);
        newIPLayer->getIPv4Header()->timeToLive = 64;


        // Create TCP layer
        pcpp::TcpLayer* newTCPLayer = new pcpp::TcpLayer(12444, destPort);

        packet.addLayer(newEthernetLayer);
        packet.addLayer(newIPLayer);
        packet.addLayer(newTCPLayer);
        // Add optional HTTP layer
        if (!uri.empty()) {
            pcpp::HttpRequestLayer* httpRequestLayer = new pcpp::HttpRequestLayer(
                pcpp::HttpRequestLayer::HttpGET, uri, pcpp::HttpVersionUnknown);
            packet.addLayer(httpRequestLayer);
        }

        // Add optional Payload layer
        if (!payload.empty()) {
            pcpp::PayloadLayer* payloadLayer = new pcpp::PayloadLayer(
                reinterpret_cast<const uint8_t*>(payload.c_str()), payload.length());
            packet.addLayer(payloadLayer);
        }

        // Finalize the packet
        packet.computeCalculateFields();

    }
    ~MockCapturedPackets() {
        // while (packet.getFirstLayer() != nullptr) {
        //     packet.removeFirstLayer(); // Remove the first layer
        // }
        // pcpp::Layer* layer = packet.getFirstLayer();
        // while (layer != nullptr) {
        //     pcpp::ProtocolType layerType = layer->getProtocol(); // Get the protocol type of the layer
        //     packet.removeLayer(layerType);  // Remove the layer by protocol type
        //     layer = packet.getFirstLayer(); // Restart iteration after removal
        // }

    }
};


TEST(AnalyzerControllerTest, AnalyzePacketForBruteForce) {
    auto model = std::make_shared<AnalyzerModel>();
    auto view = std::make_shared<AnalyzerView>();
    auto controller = std::make_shared<AnalyzerController>(model, view);
    std::vector<CapturedPackets> packets;

    for (int i = 0; i <= 22; ++i) {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer* ethLayer = new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        pcpp::IPv4Layer* ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);


        uint16_t srcPort = 12345 + i;
        uint16_t dstPort = 22;
        pcpp::TcpLayer* tcpLayer = new pcpp::TcpLayer(srcPort, dstPort);
        tcpLayer->getTcpHeader()->synFlag = 1;
        tcpLayer->getTcpHeader()->windowSize = htons(65535);
        packet.addLayer(tcpLayer);

        tcpLayer->computeCalculateFields();
        ipLayer->computeCalculateFields();
        ethLayer->computeCalculateFields();
        packets.emplace_back(
            0,
            false,
            "2024-12-15 12:00:00",
            std::move(packet),
            false
        );
    }
    for (int i = 0; i <= 19; i++) {
        controller->analyzePacketForBruteForce(packets[i]);
    }
    EXPECT_EQ(controller->getWarnings().size(), 0);
    controller->analyzePacketForBruteForce(packets[20]);
    EXPECT_EQ(controller->getWarnings().size(), 1);
}

TEST(AnalyzerControllerTest, AnalyzePacketHttp) {
    auto model = std::make_shared<AnalyzerModel>();
    auto view = std::make_shared<AnalyzerView>();
    auto controller = std::make_shared<AnalyzerController>(model, view);
    std::vector<CapturedPackets> packets;

    {
        pcpp::Packet packet(100);

        pcpp::MacAddress srcMac("01:02:03:04:05:06");
        pcpp::MacAddress dstMac("AA:BB:CC:DD:EE:FF");
        pcpp::EthLayer* ethLayer = new pcpp::EthLayer(srcMac, dstMac, PCPP_ETHERTYPE_IP);
        packet.addLayer(ethLayer);

        pcpp::IPv4Address srcIP("192.168.1.1");
        pcpp::IPv4Address dstIP("192.168.1.2");
        pcpp::IPv4Layer* ipLayer = new pcpp::IPv4Layer(srcIP, dstIP);
        ipLayer->getIPv4Header()->timeToLive = 64;
        packet.addLayer(ipLayer);


        uint16_t srcPort = 12345;
        uint16_t dstPort = 80;
        pcpp::TcpLayer* tcpLayer = new pcpp::TcpLayer(srcPort, dstPort);
        tcpLayer->getTcpHeader()->synFlag = 1;
        tcpLayer->getTcpHeader()->windowSize = htons(65535);
        packet.addLayer(tcpLayer);

        pcpp::HttpRequestLayer* httpRequestLayer = new pcpp::HttpRequestLayer(pcpp::HttpRequestLayer::HttpGET, "/index.php?id=1' OR '1'='1", pcpp::HttpVersionUnknown);
        packet.addLayer(httpRequestLayer);

        tcpLayer->computeCalculateFields();
        ipLayer->computeCalculateFields();
        ethLayer->computeCalculateFields();
        httpRequestLayer->computeCalculateFields();
        packets.emplace_back(
            0,
            false,
            "2024-12-15 12:00:00",
            std::move(packet),
            false
        );
    }
    controller->analyzePacketHttp(packets[0]);
    EXPECT_EQ(controller->getWarnings().size(), 1);

    // // Przypadek 2: Próba XSS
    // MockCapturedPackets packet2("192.168.1.20", 80, "2024-12-15 20:33:00",
    //                             "/index.php", "<script>alert('XSS')</script>");
    // controller->analyzePacketHttp(packet2);
    // EXPECT_EQ(controller->getWarnings().size(), 2);
    //
    // // Przypadek 3: Próba SQL injection i XSS
    // MockCapturedPackets packet3("192.168.1.20", 80, "2024-12-15 20:34:00",
    //                             "/index.php?id=1 UNION SELECT 1,2,3",
    //                             "<script>alert('XSS')</script>");
    // controller->analyzePacketHttp(packet3);
    // EXPECT_EQ(controller->getWarnings().size(), 3);
}
//
// TEST(AnalyzerControllerTest, AnalyzePacketForPortScan) {
//     auto model = std::make_shared<AnalyzerModel>();
//     auto view = std::make_shared<AnalyzerView>();
//     auto controller = std::make_shared<AnalyzerController>(model, view);
//
//     // Symulacja skanowania portów z jednego adresu IP
//     for (int port = 1; port <= 15; ++port) {
//         MockCapturedPackets packet("192.168.1.30", port, "2024-12-15 20:35:00");
//         controller->analyzePacketForPortScan(packet);
//     }
//
//     EXPECT_EQ(controller->getWarnings().size(), 1);
// }
//
// TEST(AnalyzerControllerTest, AnalyzeCapturedPackets) {
//     auto model = std::make_shared<AnalyzerModel>();
//     auto view = std::make_shared<AnalyzerView>();
//     auto controller = std::make_shared<AnalyzerController>(model, view);
//     std::vector<CapturedPackets> packets;
//
//     // Dodaj kilka pakietów do wektora
//     packets.push_back(MockCapturedPackets("192.168.1.40", 22, "2024-12-15 20:36:00"));
//     packets.push_back(MockCapturedPackets("192.168.1.40", 80, "2024-12-15 20:37:00", "/index.php?id=1", ""));
//     packets.push_back(MockCapturedPackets("192.168.1.40", 443, "2024-12-15 20:38:00"));
//
//     controller->analyzeCapturedPackets(packets);
//
//     EXPECT_EQ(controller->getAnalyzedPacketsCount(), 3);
//     // Dodaj więcej asercji, aby sprawdzić, czy ostrzeżenia zostały poprawnie wygenerowane
// }