#ifndef UTILS_H
#define UTILS_H

#include <Packet.h>
#include <PcapLiveDevice.h>
#include <string>
#include <iomanip>
#include <ctime>

#include <deque>

class Utils{

public:
    static Utils *getInstance();

    static void onPacketArrivesBlockingMode(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie);
    static std::string formatTimestamp(const std::time_t *tp);
    static std::string getTime();
private:
    static Utils *instance;
    Utils(){};

};


#endif

