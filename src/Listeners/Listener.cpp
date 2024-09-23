#include "listeners/Listener.h"
#include "utils/Utils.h"

#include <PcapLiveDevice.h>
#include <PcapLiveDeviceList.h>
#include <iostream>

Listener::Listener(std::shared_ptr<PacketCaptureModel> model){
    this->devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
    // this->dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByIp("192.168.0.178");
    this->dev = nullptr;
    this->model = model;
}

void Listener::openListener(){
    if (this->dev == nullptr) { 
        //WRITE TO LOG 
        std::cout << "ERROR DEVICE IS NOT SELECTED" << __PRETTY_FUNCTION__<< std::endl;
        return;
    }
    if (this->dev->isOpened()) {
        std::cout << "ERROR DEVICE IS ACTUALLY OPEN" << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;
    }
    if (this->dev->open()) {
        std::cout << "DEVICE IS OPENED AT INTERFACE: " << this->dev->getName() << std::endl;
    } else {
        std::cout << "ERROR CANT OPEN DEVICE" << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG 
    }
}

void Listener::closeListener(){
    if (this->dev == nullptr) {
        std::cout << "ERROR DEVICE IS NOT SELECTED " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;
    }
    if (!this->dev->isOpened()) {
        std::cout << "ERROR DEVICE IS NOT OPEN " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;           
    }
    std::cout << "DEVICE IS CLOSED " << __PRETTY_FUNCTION__<< std::endl;
    this->dev->close();
}

void Listener::startCapturePackets(){
    if (this->dev == nullptr){
        std::cout << "ERROR DEVICE IS NOT SELECTED " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;
    }
    if (!this->dev->isOpened()) {
        std::cout << "ERROR DEVICE IS NOT OPEN " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;
    }
    this->dev->startCapture(Utils::onPacketArrivesBlockingMode, &model);
}

void Listener::stopCapturePackets(){
    if (this->dev == nullptr){
        std::cout << "ERROR DEVICE IS NOT SELECTED " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;
    }
    if (!this->dev->isOpened()) {
        std::cout << "ERROR DEVICE IS NOT OPEN " << __PRETTY_FUNCTION__<< std::endl;
        //WRITE TO LOG
        return;
    }
    if (!this->dev->captureActive()) {
        std::cout << "ERROR DEVICE IS NOT CAPTURING " << __PRETTY_FUNCTION__<< std::endl;
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

void Listener::setDeviceByName(std::string name) {
    if (this->dev != nullptr) {
        if (this->dev->isOpened()) {
            std::cout << "DEVICE IS OPEN " << __PRETTY_FUNCTION__<< std::endl;
            this->dev->close();
            this->dev = nullptr;
        }

        if (this->dev->captureActive()) {
            std::cout << "DEVICE IS CAPTURING " << __PRETTY_FUNCTION__<< std::endl;
            return;
        }
    }
    this->dev = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(name);

    if (this->dev != nullptr) {
        std::cout << "DEVICE IS SELECTED: " << name << __PRETTY_FUNCTION__<< std::endl;
    }
}

bool Listener::isDeviceOpen() {
    if (this->dev == nullptr) {
        return false;
    }
    return this->dev->isOpened();
}

