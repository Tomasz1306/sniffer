#include "listeners/Listener.h"

#include <PcapFileDevice.h>
#include <PcapLiveDeviceList.h>

Listener::Listener(){
    this->devices = pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();


}

void Listener::openListener(){
    if (this->dev == nullptr) { 
        //WRITE TO LOG 
        return;
    }
    if (this->dev->isOpened) {
        //WRITE TO LOG
        return;
    }
    if (this->dev->open()) {
        //WRITE TO LOG
    } else {
        //WRITE TO LOG 
    }
}

void Listener::closeListener(){
    if (this->dev == nullptr) {
        //WRITE TO LOG
        return;
    }
    if (!this->dev->isOpened()) {
        //WRITE TO LOG
        return;           
    }
    if (this->dev->close()) {
        //WRITE TO LOG
    } else {
        //WRITE TO LOG
    }
}

void Listener::startCapturePackets(){
    if (this->dev == nullptr){
        //WRITE TO LOG
        return;
    }
    if (!this->dev->isOpened()) {
        //WRITE TO LOG
        return;
    }
    if (this->dev->startCapture()) {
        
    }
}