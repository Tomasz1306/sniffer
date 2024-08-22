#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <memory>

#include "views/PacketCaptureView.h"
#include "models/PacketCaptureModel.h"
#include "listeners/Listener.h"

//Forward declaration
class PacketCaptureView;

class MainController : public std::enable_shared_from_this<MainController> {
public:
    MainController(std::shared_ptr<PacketCaptureModel> _model, std::shared_ptr<PacketCaptureView> _view, std::shared_ptr<Listener> _listener);
    void run();
    void update();
    void display();

    void startCapture();
    void stopCapture();
    void clearTableOfPackets();

    std::shared_ptr<MainController> getSharedPtr() { return shared_from_this(); }
private:
    std::shared_ptr<PacketCaptureView> view;
    std::shared_ptr<PacketCaptureModel> model;
    std::shared_ptr<Listener> listener;
};

#endif // MAINCONTROLLER_H
