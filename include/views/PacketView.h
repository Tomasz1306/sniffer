//
// Created by tomasz on 9/11/24.
//

#ifndef PACKETVIEW_H
#define PACKETVIEW_H

#include "controllers/MainController.h"
#include "models/PacketModel.h"
#include "View.h"

#include <memory>
#include <string>

class MainController;

class PacketView : public View {
public:
    PacketView();
    void draw(std::shared_ptr<MainController> controller, CapturedPackets &_packet);

private:

    bool isInitialized{false};

    void keyboardHandling(std::shared_ptr<MainController> controller, CapturedPackets &_packet);
    std::string byteToHex(unsigned char byte);
};

#endif //PACKETVIEW_H
