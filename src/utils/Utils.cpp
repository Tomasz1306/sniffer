#include "utils/Utils.h"
#include "models/PacketCaptureModel.h"

#include <Packet.h>
#include <PcapLiveDevice.h>

#include <iostream>
#include <memory>
#include <ctime>
#include "controllers/LogController.h"

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

// void Utils::onPacketArrivesBlockingMode(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie){
//     auto model = static_cast<std::shared_ptr<PacketCaptureModel>*>(cookie);
//     (*model)->addToCapturedPacketDeque(pcpp::Packet(packet));
// }

std::string Utils::getTime() {
    time_t tt;
    struct tm* ti;

    time(&tt);

    ti = localtime(&tt);

    if (ti != nullptr) {
        std::string data_czas = std::to_string(ti->tm_mday) + "-" +
                                std::to_string(ti->tm_mon + 1) + "-" + // Miesiące zaczynają się od 0
                                std::to_string(ti->tm_year + 1900) + " " + // Rok zaczyna się od 1900
                                std::to_string(ti->tm_hour) + ":" +
                                std::to_string(ti->tm_min) + ":" +
                                std::to_string(ti->tm_sec);   // Rok zaczyna się od 1900, więc dodajemy 1900
        return data_czas;
    } else {
        LogController::getInstance()->addLog("--",  "ERROR WHILE READING TIME" + std::string(__PRETTY_FUNCTION__), LogType::ERROR);
        return "";
    }
}

ImFont *Utils::getLargerFont() {
    return this->largerFont;
}

ImFont *Utils::getSmallerFont() {
    return this->smallerFont;
}

void Utils::setLargerFont(ImFont *font) {
    this->largerFont = font;
}

void Utils::setSmallerFont(ImFont *font) {
    this->smallerFont = font;
}

