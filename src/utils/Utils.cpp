#include "utils/Utils.h"
#include "models/PacketCaptureModel.h"

#include <Packet.h>
#include <PcapLiveDevice.h>

#include <iostream>
#include <memory>

Utils* Utils::instance = NULL;

Utils *Utils::getInstance(){
    if (instance == NULL) {
        instance = new Utils();
        return instance;
    } else {
        return instance;
    }
}

std::string Utils::formatTimestamp(const std::time_t *tp) {
    // Konwertuj timestamp na czytelną datę i godzinę
    std::tm* ptm = std::localtime(tp);
    char timeString[32];
    std::strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", ptm);
    return timeString;
}

void Utils::onPacketArrivesBlockingMode(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie){
    auto model = static_cast<std::shared_ptr<PacketCaptureModel>*>(cookie);
    (*model)->addToCapturedPacketDeque(pcpp::Packet(packet));
}
