#ifndef PACKETCAPTUREVIEW_H
#define PACKETCAPTUREVIEW_H

#include "imgui.h"
#include "View.h"
#include "models/PacketModel.h"

#include <Device.h>
#include <Packet.h>

#include <mutex>
#include <vector>

class PacketCaptureView : public View {


public:

    PacketCaptureView () {}
    virtual ~PacketCaptureView() {}
    void draw(std::vector<CapturedPackets> &packets);

private:

    int selectedRow{-1};
    ImGuiSelectableFlags selectableRowFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick;

    void displayPacketToolTip();
    std::string parseTimeToStr(std::time_t);

    void displayTableOfPackets(std::vector<CapturedPackets>&);
    void displayIndex(CapturedPackets&);
    void displayTime(const pcpp::Packet&);
    void displayPacketSize(const pcpp::Packet&);
    void displayEthernetLayer(const pcpp::Packet&);
    void displayNetworkProtocol(const pcpp::Packet&);
    void displayTransportProtocol(const pcpp::Packet&);
    void displayTransportLayer(const pcpp::Packet&);
    void displayFlags(const pcpp::Packet&);

};

#endif // PACKETSNIFFWINDOW
