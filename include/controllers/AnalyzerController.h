//
// Created by tomasz on 02.10.24.
//

#ifndef ANALYZERCONTROLLER_H
#define ANALYZERCONTROLLER_H

#include "controllers/MainController.h"
#include "models/AnalyzerModel.h"
#include "models/PacketModel.h"
#include "views/AnalyzerView.h"
#include "global/Global.h"

#include <memory>
#include <set>
#include <mutex>
#include <thread>
#include "netinet/in.h"

class MainController;
class AnalyzerModel;
class AnalyzerView;

class Warning {
    public:
    std::string ip;
    std::string description;
    Warning(){}
    Warning(std::string ip, std::string description) : ip(ip), description(description) {} 
};

class AnalyzerController : public std::enable_shared_from_this<AnalyzerController> {
public:
    AnalyzerController(std::shared_ptr<AnalyzerModel> model, std::shared_ptr<AnalyzerView> view);

    std::shared_ptr<AnalyzerController> getController();
    void display();
    void setMainController(std::shared_ptr<MainController> controller);
    std::vector<CapturedPackets> &getPacketsToAnalyze();
    std::vector<CapturedPackets> &getCapturedVectorData();
    std::unordered_map<std::string, Warning> &getWarnings();
    void analyzePackets(std::vector<CapturedPackets> packets);

    void analyzeCapturedPackets(std::vector<CapturedPackets> packets);
    int getPacketsToAnalyzeCount() { return this->packetToAnalyzeCount; }
    int getAnalyzedPacketsCount() {return this->analyzedPacketsCount; }
    void setIsAnalyzingEnable(bool isAnalyzingEnable) { this->isAnalyzingEnable = isAnalyzingEnable; }
    bool getIsAnalyzingEnable() {return this->isAnalyzingEnable; }

    void threadAnalyzer();
    void analyzePacketForPortScan(CapturedPackets packet);
    void analyzePacketForBruteForce(CapturedPackets packet);
    void analyzePacketForARPSpoofing(CapturedPackets packet);
    void analyzePacketForPingSweep(CapturedPackets packet);

private:
    std::shared_ptr<std::thread> analyzerThread;
    std::shared_ptr<MainController> mainController;
    std::shared_ptr<AnalyzerModel> model;
    std::shared_ptr<AnalyzerView> view;
    std::vector<CapturedPackets> packetsToAnalyze;
    std::unordered_map<std::string, std::set<uint16_t>> sourceIpToPorts;
    std::unordered_map<std::string, Warning> warnings;
    std::unordered_map<std::string, int> bruteForceAttempts; // IP:Date to count attempts
    std::unordered_map<std::string, std::string> arpMapping; // IP to MAC mapping
    std::unordered_map<std::string, int> pingRequests;       // IP to Ping request count
    bool isAnalyzingEnable{false};
    int packetToAnalyzeCount{0};
    int analyzedPacketsCount{0};
};

#endif //ANALYZERCONTROLLER_H
