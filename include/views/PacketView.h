//
// Created by tomasz on 9/11/24.
//

#ifndef PACKETVIEW_H
#define PACKETVIEW_H
#include "View.h"
#include "models/PacketModel.h"

class PacketView : public View {
public:

    void draw(CapturedPackets &_packet);

private:

    bool isInitialized{false};
};

#endif //PACKETVIEW_H
