//
// Created by tomasz on 02.10.24.
//

#include "controllers/AnalyzerController.h"
AnalyzerController::AnalyzerController(std::shared_ptr<AnalyzerModel> model, std::shared_ptr<AnalyzerView> view) {
    this->model = model;
    this->view = view;
}

void AnalyzerController::threadAnalyzer() {

    if (!this->packetsToAnalyze.empty()) {
        this->analyzeCapturedPackets();
    }
}

std::vector<CapturedPackets> &AnalyzerController::getPacketsToAnalyze() {
    return this->packetsToAnalyze;
}

void AnalyzerController::analyzePackets(std::vector<CapturedPackets> packets) {
    this->packetsToAnalyze.assign(packets.begin(), packets.end());
    analyzerGuard.lock();
    this->packetToAnalyzeCount = this->packetsToAnalyze.size();
    analyzerGuard.unlock();
    this->analyzerThread = std::make_shared<std::thread>(&AnalyzerController::threadAnalyzer, this);
    this->analyzerThread->detach();
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

std::unordered_map<std::string, std::string> &AnalyzerController::getWarnings() {
    return this->warnings;
}

std::vector<CapturedPackets> &AnalyzerController::getCapturedVectorData() {
    return this->mainController->getCapturedVectorData();
}

void AnalyzerController::analyzePacketForPingSweep(CapturedPackets packet) {
    auto icmpLayer = packet.packet.getLayerOfType<pcpp::IcmpLayer>();
    if (icmpLayer != nullptr && icmpLayer->getMessageType() == pcpp::ICMP_ECHO_REQUEST) {
        std::string sourceIp = packet.packet.getLayerOfType<pcpp::IPv4Layer>()->getSrcIPAddress().toString();
        pingRequests[sourceIp]++;
        if (pingRequests[sourceIp] > 10 && warnings.find(sourceIp) == warnings.end()) {
            std::ostringstream oss;
            oss << "Potential Ping Sweep detected from IP: " << sourceIp;
            warnings[sourceIp] = oss.str();
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
            warnings[srcIP] = oss.str();
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
                warnings[sourceIp] = oss.str();
            }
        }
    }
}

void AnalyzerController::analyzePacketForPortScan(CapturedPackets packet) {
    auto ipLayer = packet.packet.getLayerOfType<pcpp::IPv4Layer>();
    auto tcpLayer = packet.packet.getLayerOfType<pcpp::TcpLayer>();
    if (ipLayer != nullptr && tcpLayer != nullptr) {
        std::string sourceIp = ipLayer->getSrcIPAddress().toString();
        uint16_t destPort = ntohs(tcpLayer->getTcpHeader()->portDst);
        sourceIpToPorts[sourceIp].insert(destPort);
        if (sourceIpToPorts[sourceIp].size() > 10 && warnings.find(sourceIp) == warnings.end()) {
            std::ostringstream oss;
            oss << "Potential port scan detected from IP: " << sourceIp << " " << "Date: " << packet.captureTime;
            warnings[sourceIp] = oss.str();
        }
    }
}