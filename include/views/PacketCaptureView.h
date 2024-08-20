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

private:
    void displayPacketToolTip();
    std::string parseTimeToStr(std::time_t);

    void displayTime(const pcpp::Packet&);
    void displayPacketSize(const pcpp::Packet&);
    void displayEthernetLayer(const pcpp::Packet&);
    void displayNetworkProtocol(const pcpp::Packet&);
    void displayTransportProtocol(const pcpp::Packet&);
    void displayTransportLayer(const pcpp::Packet&);
    void displayFlags(const pcpp::Packet&);

};

#endif // PACKETSNIFFWINDOW
