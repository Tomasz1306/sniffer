//
// Created by tomasz on 9/11/24.
//

#ifndef PACKETVIEW_H
#define PACKETVIEW_H
#include "View.h"
#include "models/PacketModel.h"
#include "controllers/MainController.h"
#include <memory>
class MainController;

class PacketView : public View {
public:

    void draw(std::shared_ptr<MainController> controller, CapturedPackets &_packet);

private:

    bool isInitialized{false};

    void displayEthernetLayer(CapturedPackets &_packet);
    void displayNetworkLayer(CapturedPackets &_packet);
    void displayTransportLayer(CapturedPackets &_packet);
    void displayApplicationLayer(CapturedPackets &_packet);
};

#endif //PACKETVIEW_H
