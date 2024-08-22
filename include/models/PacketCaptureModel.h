#ifndef PACKETCAPTUREMODEL_H
#define PACKETCAPTUREMODEL_H

#include "models/Model.h"
#include "models/PacketModel.h"

#include <thread>
#include <memory>
#include <deque>

#include <PacketUtils.h>
#include <PcapLiveDevice.h>

class PacketCaptureModel : public Model {

public:
    PacketCaptureModel();

    virtual ~PacketCaptureModel();

    std::vector<CapturedPackets> &getCapturedPacketVector();
    void addToCapturedPacketDeque(pcpp::Packet packet);

    void writeFromDequeToVector();

    void clearCapturedPacketVector();

private:

    int counter{0};

    std::shared_ptr<std::thread> thread_1;
    std::vector<CapturedPackets> capturedPackets_vector;
    std::deque<pcpp::Packet> capturedPackets_deque;


private:



};


#endif
