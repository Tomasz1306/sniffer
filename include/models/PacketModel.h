//
// Created by tomasz on 8/20/24.
//

#ifndef PACKETMODEL_H
#define PACKETMODEL_H

#include <string>
#include <utility>

#include <Packet.h>

struct CapturedPackets {
    int id;
    bool selected;
    bool isOpen;
    std::string captureTime;
    pcpp::Packet packet; // Potential ownership issues
    bool test;

    // Default Constructor
    CapturedPackets() : id(0), selected(false), isOpen(false), captureTime(""), packet(1024), test(false) {}
    CapturedPackets(int _id, bool _selected, std::string _capturedTime, pcpp::Packet _packet);
    // Parameterized Constructor
    // CapturedPackets(int _id, bool _selected, std::string _capturedTime, const pcpp::Packet& _packet)
    //     : id(_id), selected(_selected), isOpen(false), captureTime(std::move(_capturedTime)), packet(_packet), test(false) {}

    CapturedPackets(int _id, bool _selected, std::string _capturedTime, pcpp::Packet&& _packet, bool _test)
        : id(_id), selected(_selected), isOpen(false), captureTime(std::move(_capturedTime)), packet(std::move(_packet)), test(_test) {}

};

#endif //PACKETMODEL_H