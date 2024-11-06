//
// Created by tomasz on 02.10.24.
//

#ifndef ANALYZERCONTROLLER_H
#define ANALYZERCONTROLLER_H

#include "models/AnalyzerModel.h"
#include "views/AnalyzerView.h"
#include "controllers/MainController.h"
#include "global/Global.h"

class MainController;
class AnalyzerModel;
class AnalyzerView;

#include <memory>
#include <set>
#include <mutex>
#include <thread>
#include "netinet/in.h"

class AnalyzerController : public std::enable_shared_from_this<AnalyzerController> {
public:
    AnalyzerController(std::shared_ptr<AnalyzerModel> model, std::shared_ptr<AnalyzerView> view);

    std::shared_ptr<AnalyzerController> getController();
    void display();
    void setMainController(std::shared_ptr<MainController> controller);
    std::vector<CapturedPackets> &getPacketsToAnalyze();
    std::vector<CapturedPackets> &getCapturedVectorData();
    std::unordered_map<std::string, std::string> &getWarnings();
    void analyzePackets(std::vector<CapturedPackets> packets);

    void analyzeCapturedPackets() {
        for (int i =0 ; i < packetsToAnalyze.size(); i++) {
            analyzePacketForPortScan(packetsToAnalyze[i]);
            analyzePacketForBruteForce(packetsToAnalyze[i]);
            //analyzePacketForARPSpoofing(packets[i]);
            //analyzePacketForPingSweep(packets[i]);
            packetsToAnalyze.erase(packetsToAnalyze.begin() + i);
            std::lock_guard<std::mutex> lock(analyzerGuard);
            analyzedPacketsCount++;
        }
        {
            std::lock_guard<std::mutex> lock(analyzerGuard);
            packetToAnalyzeCount = 0;
        }
        this->isAnalyzingEnable = false;
    }
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
    std::unordered_map<std::string, std::string> warnings;
    std::unordered_map<std::string, int> bruteForceAttempts; // IP:Date to count attempts
    std::unordered_map<std::string, std::string> arpMapping; // IP to MAC mapping
    std::unordered_map<std::string, int> pingRequests;       // IP to Ping request count
    bool isAnalyzingEnable{false};
    int packetToAnalyzeCount{0};
    int analyzedPacketsCount{0};
};

#endif //ANALYZERCONTROLLER_H
