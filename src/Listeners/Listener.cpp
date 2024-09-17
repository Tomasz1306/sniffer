#include "listeners/Listener.h"
#include "utils/Utils.h"

#include <PcapLiveDevice.h>
#include <PcapLiveDeviceList.h>
#include <iostream>

Listener::Listener(std::shared_ptr<PacketCaptureModel> model){
    this->devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
    this->dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp("192.168.0.178");
    this->model = model;
}

void Listener::openListener(){
    if (this->dev == nullptr) { 
        //WRITE TO LOG 
        std::cout << "ERROR " << __PRETTY_FUNCTION__<< std::endl;
        return;
    }
    if (this->dev->isOpened()) {
        std::cout << "ERROR " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;
    }
    if (this->dev->open()) {
        //WRITE TO LOG
    } else {
        std::cout << "ERROR " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG 
    }
}

void Listener::closeListener(){
    if (this->dev == nullptr) {
        std::cout << "ERROR " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;
    }
    if (!this->dev->isOpened()) {
        std::cout << "ERROR " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;           
    }
    this->dev->close();
}

void Listener::startCapturePackets(){
    if (this->dev == nullptr){
        std::cout << "ERROR " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;
    }
    if (!this->dev->isOpened()) {
        std::cout << "ERROR " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;
    }
    this->dev->startCapture(Utils::onPacketArrivesBlockingMode, &model);
}

void Listener::stopCapturePackets(){
    if (!this->dev->captureActive()) {
        std::cout << "ERROR " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG 
        return;
    }
    this->dev->stopCapture();
}

void Listener::setFilters(pcpp::AndFilter filter) {
    if (this->dev == nullptr) {
        std::cout << "ERROR " << __PRETTY_FUNCTION__<< std::endl;
        return;
    }
    if (this->dev->captureActive()) {
        this->stopCapturePackets();
        this->dev->setFilter(filter);
        this->startCapturePackets();
    }
    if (!this->dev->captureActive()) {
        this->dev->setFilter(filter);
    }
}