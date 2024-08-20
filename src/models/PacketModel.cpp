//
// Created by tomasz on 8/20/24.
//
#include "models/PacketModel.h"

CapturedPackets::CapturedPackets(int _id, bool _selected, std::string _capturedTime, pcpp::Packet _packet) {
    this->id = _id;
    this->selected = _selected;
    this->captureTime = _capturedTime;
    this->packet = _packet;
}
