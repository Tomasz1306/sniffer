#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <memory>

#include "views/PacketCaptureView.h"
#include "views/PacketView.h"
#include "models/PacketCaptureModel.h"
#include "listeners/Listener.h"
#include "views/PacketView.h"


//Forward declaration
class PacketCaptureView;
class PacketView;

class MainController : public std::enable_shared_from_this<MainController> {
public:
    MainController(std::shared_ptr<PacketCaptureModel> _model,
        std::shared_ptr<PacketCaptureView> _view,
        std::shared_ptr<Listener> _listener,
        std::shared_ptr<PacketView> _packetView);
    void run();
    void update();
    void display();

    void startCapture();
    void stopCapture();
    void clearTableOfPackets();
    void setIsDisplayedPakcet(bool _isDisplayedPakcet);
    void setCurrentPacketId(int id);

    std::shared_ptr<MainController> getSharedPtr() { return shared_from_this(); }
private:

    bool isPacketDisplayed{false};

    std::shared_ptr<PacketCaptureView> view;
    std::shared_ptr<PacketCaptureModel> model;
    std::shared_ptr<Listener> listener;
    std::shared_ptr<PacketView> packetView;
};

#endif // MAINCONTROLLER_H
