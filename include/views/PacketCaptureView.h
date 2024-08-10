#ifndef PACKETCAPTUREVIEW_H
#define PACKETCAPTUREVIEW_H

#include "imgui.h"
#include "View.h"

#include <Device.h>
#include <Packet.h>

#include <mutex>
#include <vector>

class PacketCaptureView : public View {


public:

    PacketCaptureView () {}
    virtual ~PacketCaptureView() {}
    void draw(const std::vector<pcpp::Packet> &packets);

};


#endif // PACKETSNIFFWINDOW
