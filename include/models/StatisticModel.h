//
// Created by tomasz on 01.10.24.
//

#ifndef STATISTICMODEL_H
#define STATISTICMODEL_H

#include <string>
#include "Packet.h"
#include <deque>
class StatisticModel {
public:

    StatisticModel(std::string date);

    void addPacketToStatistics(pcpp::Packet& packet);

    std::string date;
    int ethernetCount{0};
    int arpCount{0};
    int imcpv4Count{0};
    int ipv4Count{0};
    int ipv6Count{0};
    int tcpCount{0};
    int udpCount{0};
    int sslTslCount{0};
    int dhcpv4Count{0};
    int dnsCount{0};
    int ftpCount{0};
    int httpCount{0};
    int sshCount{0};
    int telnetCount{0};

};


#endif //STATISTICMODEL_H
