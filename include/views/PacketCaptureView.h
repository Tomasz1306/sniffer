#ifndef PACKETCAPTUREVIEW_H
#define PACKETCAPTUREVIEW_H

#include "controllers/MainController.h"
#include "models/PacketModel.h"
#include "View.h"

#include <vector>
#include <ctime>

#include "imgui.h"
#include "imgui_internal.h"

class MainController;

class PacketCaptureView : public View {

    enum class captureState {
        STOP,
        START
    };

public:

    PacketCaptureView();
    virtual ~PacketCaptureView() {}
    void draw(std::shared_ptr<MainController> controller, std::vector<CapturedPackets> &packets);

    int getCurrentSelectedPacketId() const;
    void setCurrentSelectedPacketId(int id);

private:

    int selectedRow{-1};
    int singleSelectRow{-1};
    std::string captureButtonText{"START"};
    captureState captureButtonState{captureState::STOP};

    ImGuiSelectableFlags selectableRowFlags = ImGuiSelectableFlags_SpanAllColumns | ImGuiSelectableFlags_SelectOnClick | ImGuiMultiSelectFlags_SingleSelect;

    void displayPacketToolTip();
    std::string parseTimeToStr(std::time_t);
    bool isDownKeyPressed = false;
    bool isUpKeyPressed  = false;
    bool autoScroll = true;
    void displayMenuBar();
    //TODO zmienic nazwe na bardziej odpowiadajaca dla displayOption
    void displayOption(std::shared_ptr<MainController> controller);
    void displayTableOfPackets(std::vector<CapturedPackets>&, std::shared_ptr<MainController> controller);
    void displayIndex(CapturedPackets&, std::shared_ptr<MainController> controller);
    void displayTime(const pcpp::Packet&);
    void displayPacketSize(const pcpp::Packet&);
    void displayDataLinkLayer(const pcpp::Packet&);
    void displayNetworkProtocol(const pcpp::Packet&);
    void displayTransportProtocol(const pcpp::Packet&);
    void displayTransportLayer(const pcpp::Packet&);
    void displayFlags(const pcpp::Packet&);
    void keyboardHandling(std::shared_ptr<MainController> controller,std::vector<CapturedPackets> &packets, CapturedPackets &packet);
};

#endif // PACKETSNIFFWINDOW
