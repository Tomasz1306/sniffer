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
    std::vector<CapturedPackets> &getCapturedPacketsVectorAnalyze();
    std::unordered_map<std::string, std::string> &getWarnings();

    void analyzeCapturedPackets(std::vector<CapturedPackets> &packets) {
        for (int i =0 ; i < packets.size(); i++) {
            analyzePacketForPortScan(packets[i]);
            analyzePacketForBruteForce(packets[i]);
            //analyzePacketForARPSpoofing(packets[i]);
            //analyzePacketForPingSweep(packets[i]);
            std::lock_guard<std::mutex> lock(analyzerGuard);
            packets.erase(packets.begin() + i);
        }
    }
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
    std::unordered_map<std::string, std::set<uint16_t>> sourceIpToPorts;
    std::unordered_map<std::string, std::string> warnings;
    std::unordered_map<std::string, int> bruteForceAttempts; // IP:Date to count attempts
    std::unordered_map<std::string, std::string> arpMapping; // IP to MAC mapping
    std::unordered_map<std::string, int> pingRequests;       // IP to Ping request count
    bool isAnalyzingEnable{false};
};

#endif //ANALYZERCONTROLLER_H
