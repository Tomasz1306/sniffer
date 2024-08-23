#ifndef PACKETCAPTUREVIEW_H
#define PACKETCAPTUREVIEW_H

#include "imgui.h"
#include "View.h"
#include "models/PacketModel.h"
#include "controllers/MainController.h"

#include <vector>
#include <ctime>


class MainController;

class PacketCaptureView : public View {

    enum class captureState {
        STOP,
        START
    };

public:

    PacketCaptureView () {}
    virtual ~PacketCaptureView() {}
    void draw(std::shared_ptr<MainController> controller, std::vector<CapturedPackets> &packets);

private:

    int selectedRow{-1};
    std::string captureButtonText{"START"};
    captureState captureButtonState{captureState::STOP};


    ImGuiSelectableFlags selectableRowFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_AllowDoubleClick;

    void displayPacketToolTip();
    std::string parseTimeToStr(std::time_t);

    void displayMenuBar();
    //TODO zmienic nazwe na bardziej odpowiadajaca dla displayOption
    void displayOption(std::shared_ptr<MainController> controller);
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
