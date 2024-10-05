#ifndef UTILS_H
#define UTILS_H

#include <Packet.h>
#include <PcapLiveDevice.h>
#include <string>
#include <iomanip>
#include <ctime>
#include "imgui.h"

#include <deque>

class Utils{

public:
    static Utils *getInstance();

    static void onPacketArrivesBlockingMode(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie);
    static std::string formatTimestamp(const std::time_t *tp);
    static std::string getTime();

    void setSmallerFont(ImFont *font);
    void setLargerFont(ImFont *font);
    ImFont *getSmallerFont();
    ImFont *getLargerFont();
private:
    static Utils *instance;
    Utils(){};

    ImFont *smallerFont;
    ImFont *largerFont;

};


#endif

