#ifndef PACKETCAPTUREMODEL_H
#define PACKETCAPTUREMODEL_H

#include "controllers/MainController.h"
#include "models/Model.h"
#include "models/PacketModel.h"

#include <thread>
#include <memory>
#include <deque>
#include <mutex>
#include <condition_variable>

#include <PacketUtils.h>
#include <PcapLiveDevice.h>

class MainController;

class PacketCaptureModel : public Model {

public:
    PacketCaptureModel();

    virtual ~PacketCaptureModel();

    std::vector<CapturedPackets> &getCapturedPacketVector();
    std::vector<CapturedPackets> &getCapturedPacketVectorDatabase();
    std::vector<CapturedPackets> &getCapturedPacketToAnalyze();
    void addToCapturedPacketDeque(pcpp::Packet packet);
    void writeFromDequeToVector();
    void clearCapturedPacketVector();
    void clearCapturedPacketVectorDatabase();
    void setMainController(std::shared_ptr<MainController> controller);

private:

    int counter{0};

    std::shared_ptr<std::thread> thread_1;
    std::vector<CapturedPackets> capturedPackets_vector;
    std::deque<pcpp::Packet> capturedPackets_deque;
    std::vector<CapturedPackets> capturedPackets_database;
    std::vector<CapturedPackets> capturedPackets_analize;
    std::shared_ptr<MainController> controller;
    std::mutex dequeGuard;
    std::condition_variable dequeCV;
};


#endif
