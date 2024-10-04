#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <memory>

#include "views/PacketCaptureView.h"
#include "views/PacketView.h"
#include "models/PacketCaptureModel.h"
#include "listeners/Listener.h"
#include "controllers/StatisticController.h"
#include "PcapLiveDevice.h"
#include "views/PacketView.h"


//Forward declaration
class StatisticController;
class PacketCaptureModel;
class PacketCaptureView;
class PacketView;
class Listener;

class MainController : public std::enable_shared_from_this<MainController> {
public:
    MainController(std::shared_ptr<PacketCaptureModel> _model,
        std::shared_ptr<PacketCaptureView> _view,
        std::shared_ptr<Listener> _listener,
        std::shared_ptr<PacketView> _packetView,
        std::shared_ptr<StatisticController> _statisticController);
    void display();

    static void onPacketArrivesBlockingMode(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie);

    void startCapture();
    void stopCapture();
    void clearTableOfPackets();
    void setIsDisplayedPakcet(bool _isDisplayedPakcet);
    bool isDisplayedPakcet();
    void setCurrentPacketId(int _id);
    void displayNextPacket();
    void displayPrevPacket();
    bool isDeviceOpen();
    int getPacketCapturedVectorSize();
    void addPacketToStatistics(pcpp::Packet &packet);
    bool isDeviceSelected();

    std::shared_ptr<MainController> getSharedPtr() { return shared_from_this(); }
private:

    bool isPacketDisplayed{false};

    std::shared_ptr<PacketCaptureView> view;
    std::shared_ptr<PacketCaptureModel> model;
    std::shared_ptr<Listener> listener;
    std::shared_ptr<PacketView> packetView;
    std::shared_ptr<StatisticController> statisticController;
};

#endif // MAINCONTROLLER_H
