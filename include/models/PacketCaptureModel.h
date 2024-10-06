#ifndef PACKETCAPTUREMODEL_H
#define PACKETCAPTUREMODEL_H

#include "models/Model.h"
#include "models/PacketModel.h"
#include "controllers/MainController.h"

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
    void addToCapturedPacketDeque(pcpp::Packet packet);
    void writeFromDequeToVector();
    void clearCapturedPacketVector();
    void setController(std::shared_ptr<MainController> controller);

private:

    int counter{0};

    std::shared_ptr<std::thread> thread_1;
    std::vector<CapturedPackets> capturedPackets_vector;
    std::vector<pcpp::Packet> capturedPackets_deque;
    std::shared_ptr<MainController> controller;
    std::mutex dequeGuard;
    std::condition_variable dequeCV;
};


#endif
