//
// Created by tomasz on 01.10.24.
//

#include "models/StatisticModel.h"

StatisticModel::StatisticModel(std::string date) {
    this->date = date;
}

void StatisticModel::addPacketToStatistics(pcpp::Packet &packet) {
    // if (packet.isPacketOfType(pcpp::Ethernet))
    //     ethernetCount++;

    if (packet.isPacketOfType(pcpp::ARP))
        arpCount++;
    else if (packet.isPacketOfType(pcpp::ICMP))
        imcpv4Count++;
    else if (packet.isPacketOfType(pcpp::TCP))
        tcpCount++;
    else if (packet.isPacketOfType(pcpp::UDP))
        udpCount++;
    else if (packet.isPacketOfType(pcpp::DHCP))
        dhcpv4Count++;
    else if (packet.isPacketOfType(pcpp::DNS))
        dnsCount++;
    else if (packet.isPacketOfType(pcpp::FTP))
        ftpCount++;
    else if (packet.isPacketOfType(pcpp::HTTP))
        httpCount++;
    else if (packet.isPacketOfType(pcpp::SSH))
        sshCount++;
    else if (packet.isPacketOfType(pcpp::Telnet))
        telnetCount++;
    else if (packet.isPacketOfType(pcpp::IPv4))
        ipv4Count++;
    else if (packet.isPacketOfType(pcpp::IPv6))
        ipv6Count++;
}