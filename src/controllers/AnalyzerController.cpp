//
// Created by tomasz on 02.10.24.
//

#include <regex>

#include "controllers/AnalyzerController.h"
AnalyzerController::AnalyzerController(std::shared_ptr<AnalyzerModel> model, std::shared_ptr<AnalyzerView> view) {
    this->model = model;
    this->view = view;
}

// void AnalyzerController::threadAnalyzer() {

//     if (!this->packetsToAnalyze.empty()) {
//         this->analyzeCapturedPackets();
//     }
// }

std::vector<CapturedPackets> &AnalyzerController::getPacketsToAnalyze() {
    return this->packetsToAnalyze;
}

void AnalyzerController::analyzePackets(std::vector<CapturedPackets> packets) {
    this->analyzerThread = std::make_shared<std::thread>(&AnalyzerController::analyzeCapturedPackets, this, packets);
    this->analyzerThread->detach();
}

void AnalyzerController::analyzeCapturedPackets(std::vector<CapturedPackets> packets) {
    for (int i =0 ; i < packets.size(); i++) {
            analyzePacketForPortScan(packets[i]);
            analyzePacketForBruteForce(packets[i]);
            analyzePacketHttp(packets[i]);
            analyzedPacketsCount++;
    }
    sourceIpToPorts.clear();
}

std::shared_ptr<AnalyzerController> AnalyzerController::getController() {
    return shared_from_this();
}

void AnalyzerController::display() {
    this->view->draw(shared_from_this());
}

void AnalyzerController::setMainController(std::shared_ptr<MainController> controller) {
    this->mainController = controller;
}

std::unordered_map<std::string, Warning> &AnalyzerController::getWarnings() {
    return this->warnings;
}

std::vector<CapturedPackets> &AnalyzerController::getCapturedVectorData() {
    return this->mainController->getCapturedVectorToAnalyze();
}

void AnalyzerController::analyzePacketForPortScan(CapturedPackets packet) {
    auto ipLayer = packet.packet.getLayerOfType<pcpp::IPv4Layer>();
    auto tcpLayer = packet.packet.getLayerOfType<pcpp::TcpLayer>();
    if (ipLayer != nullptr && tcpLayer != nullptr) {
        std::string sourceIp = ipLayer->getSrcIPAddress().toString();
        uint16_t destPort = ntohs(tcpLayer->getTcpHeader()->portDst);
        sourceIpToPorts[sourceIp].insert(destPort);
        if (sourceIpToPorts[sourceIp].size() > 10) {
            std::ostringstream oss;
            std::cout << " UWAGA  " << std::endl;
            oss << "Potential port scan detected from IP: " << sourceIp << " " << "Date: " << packet.captureTime;
            warnings[packet.captureTime] = Warning(sourceIp, oss.str());
        }
    }
}

void AnalyzerController::analyzePacketForPingSweep(CapturedPackets packet) {
    auto icmpLayer = packet.packet.getLayerOfType<pcpp::IcmpLayer>();
    if (icmpLayer != nullptr && icmpLayer->getMessageType() == pcpp::ICMP_ECHO_REQUEST) {
        std::string sourceIp = packet.packet.getLayerOfType<pcpp::IPv4Layer>()->getSrcIPAddress().toString();
        pingRequests[sourceIp]++;
        if (pingRequests[sourceIp] > 10 && warnings.find(sourceIp) == warnings.end()) {
            std::ostringstream oss;
            oss << "Potential Ping Sweep detected from IP: " << sourceIp;
            warnings[packet.captureTime] = Warning(sourceIp, oss.str());
        }
    }
}

void AnalyzerController::analyzePacketForARPSpoofing(CapturedPackets packet) {
    auto arpLayer = packet.packet.getLayerOfType<pcpp::ArpLayer>();
    if (arpLayer != nullptr) {
        std::string srcIP = arpLayer->getSenderIpAddr().toString();
        std::string srcMac = arpLayer->getSenderMacAddress().toString();
        std::string dstIP = arpLayer->getTargetIpAddr().toString();
        std::string dstMac = arpLayer->getTargetMacAddress().toString();

        // Check if IP mapping to MAC address changes
        if (arpMapping[srcIP] != srcMac) {
            std::ostringstream oss;
            oss << "Potential ARP spoofing detected: IP " << srcIP << " now maps to MAC " << srcMac
                << " (was " << arpMapping[srcIP] << ")";
            warnings[packet.captureTime] = Warning(srcIP, oss.str());
        }

        // Update ARP mapping
        arpMapping[srcIP] = srcMac;
    }
}

void AnalyzerController::analyzePacketForBruteForce(CapturedPackets packet) {
    auto ipLayer = packet.packet.getLayerOfType<pcpp::IPv4Layer>();
    auto tcpLayer = packet.packet.getLayerOfType<pcpp::TcpLayer>();
    if (ipLayer != nullptr && tcpLayer != nullptr) {
        std::string sourceIp = ipLayer->getSrcIPAddress().toString();
        uint16_t destPort = ntohs(tcpLayer->getTcpHeader()->portDst);

        if (destPort == 22) { // Port 22 dla SSH
            std::string key = sourceIp + ":" + packet.captureTime;
            bruteForceAttempts[key]++;
            if (bruteForceAttempts[key] > 30 && warnings.find(key) == warnings.end()) {
                std::ostringstream oss;
                oss << "Potential SSH brute force attempt from IP: " << sourceIp << " on " << packet.captureTime;
                warnings[packet.captureTime] = Warning(sourceIp, oss.str());
            }
        }
    }
}

void AnalyzerController::analyzePacketHttp(CapturedPackets packet) {
    auto httpLayer = packet.packet.getLayerOfType<pcpp::HttpRequestLayer>();
    auto rawPacket = packet.packet.getRawPacket();

    if (httpLayer != nullptr) {
        auto firstLine = httpLayer->getFirstLine();
        std::string uri = firstLine->getUri();
        std::string info = httpLayer->toString();
        std::cout << uri << std::endl;
        std::stringstream asciiStream;
        std::string sourceIp = packet.packet.getLayerOfType<pcpp::IPv4Layer>()->getSrcIPAddress().toString();
        pcpp::PayloadLayer* payloadLayer = packet.packet.getLayerOfType<pcpp::PayloadLayer>();
        if (payloadLayer !=nullptr) {
            const uint8_t *payload = packet.packet.getLayerOfType<pcpp::PayloadLayer>()->getPayload();
            int payloadSize = packet.packet.getLayerOfType<pcpp::PayloadLayer>()->getPayloadLen();
            if (payload) {
                for (int i = 0; i < payloadSize; i++) {
                    unsigned char byte = payload[i];
                    std::stringstream hexStream;
                    asciiStream << (isprint(byte) ? static_cast<char>(byte) : '.');
                }
            }
        }
        bool isXSS{false}, isSQL{false};
        std::cout << asciiStream.str() << std::endl;
        std::string asciiString = asciiStream.str();
        std::regex xssPattern(R"(<script>)", std::regex_constants::icase);
        std::regex sql_injection_pattern(
            R"((?:'|\bOR\b|\bAND\b|\bSELECT\b|\bUNION\b|\bINSERT\b|\bUPDATE\b|
        \bDELETE\b|--|#|/\*|\*/|;|=|LIKE|\\b[0-9]+=[0-9]+\\b))");
        if (std::regex_search(uri, sql_injection_pattern) |
            std::regex_search(asciiString, sql_injection_pattern)) {
            sqlInjectionAttempts[sourceIp]++;
            isSQL = true;
        }

        if (std::regex_search(asciiString, xssPattern)) {
            xssAttempts[sourceIp]++;
            isXSS = true;
        }


        if (isXSS && isSQL) {
            warnings[packet.captureTime] = Warning(sourceIp, std::string("Potential SQL Injection and XSS attempt: " + asciiString + " at " + packet.captureTime));
        } else if (isXSS) {
            warnings[packet.captureTime] = Warning(sourceIp, std::string("Potential XSS attempt: " + asciiString + " at " + packet.captureTime));
        } else {
            warnings[packet.captureTime] = Warning(sourceIp, std::string("Potential SQL Injection attempt: " + asciiString + " at " + packet.captureTime));
        }
    } else {
    }
}
