#ifndef PACKETCAPTUREMODEL_H
#define PACKETCAPTUREMODEL_H

#include "models/Model.h"

#include <mutex>
#include <thread>
#include <memory>
#include <deque>

#include <RawPacket.h>
#include <PacketUtils.h>
#include <Device.h>
#include <PcapLiveDevice.h>

class PacketCaptureModel : public Model {


public:
    PacketCaptureModel();

    virtual ~PacketCaptureModel();

    std::vector<pcpp::Packet> &getCapturedPacketVector();
    void addToCapturedPacketDeque(pcpp::Packet packet);

    void writeFromDequeToVector();

private:

    std::shared_ptr<std::thread> thread_1;

    std::vector<pcpp::Packet> capturedPackets_vector;
    std::deque<pcpp::Packet> capturedPackets_deque;


private:



};


#endif
