#ifndef LISTENER_H
#define LISTENER_H

#include "models/Model.h"
#include "models/PacketCaptureModel.h"


#include <PcapFileDevice.h>
#include <PcapLiveDevice.h>

#include <vector>
#include <thread>
#include <memory>

class Listener{ 

public:
    Listener(std::shared_ptr<PacketCaptureModel> model);

    void openListener();
    void closeListener();
    void startCapturePackets();
    void stopCapturePackets();

    ~Listener() {};

private:

    std::shared_ptr<PacketCaptureModel> model;
    
    pcpp::PcapLiveDevice *dev{nullptr};

    std::vector<pcpp::PcapLiveDevice *> devices{nullptr};
    std::unique_ptr<std::thread> listener_thread;


};

#endif