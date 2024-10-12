//
// Created by tomasz on 8/20/24.
//

#ifndef PACKETMODEL_H
#define PACKETMODEL_H

#include <string>

#include <Packet.h>

struct CapturedPackets {
    int id;
    bool selected;
    bool isOpen;
    std::string captureTime;
    pcpp::Packet packet;

    CapturedPackets() {};
    CapturedPackets(int _id, bool _selected, std::string _capturedTime, pcpp::Packet _packet);
};

#endif //PACKETMODEL_H
