#ifndef PACKETCAPTUREMODEL_H
#define PACKETCAPTUREMODEL_H

#include "models/Model.h"

#include <RawPacket.h>
#include <PacketUtils.h>
#include <Device.h>

class PacketCaptureModel : public Model {


public:
    PacketCaptureModel();

    virtual ~PacketCaptureModel(){};
    

private:
    pcpp::RawPacketVector packets;
};


#endif