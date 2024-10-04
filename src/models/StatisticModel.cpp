//
// Created by tomasz on 01.10.24.
//

#include "models/StatisticModel.h"

StatisticModel::StatisticModel(std::string date) {
    this->date = date;
}

void StatisticModel::addPacketToStatistics(pcpp::Packet &packet) {
    if (packet.isPacketOfType(pcpp::Ethernet))
        ethernetCount++;
    if (packet.isPacketOfType(pcpp::ARP))
        arpCount++;
    if (packet.isPacketOfType(pcpp::ICMP))
        imcpv4Count++;
    if (packet.isPacketOfType(pcpp::ICMPv6))
        imcpv6Count++;
    if (packet.isPacketOfType(pcpp::IGMP))
        igmpCount++;
    if (packet.isPacketOfType(pcpp::IPv4))
        ipv4Count++;
    if (packet.isPacketOfType(pcpp::IPv6))
        ipv6Count++;
    if (packet.isPacketOfType(pcpp::TCP))
        tcpCount++;
    if (packet.isPacketOfType(pcpp::UDP))
        udpCount++;
    if (packet.isPacketOfType(pcpp::SDP))
        sdpCount++;
    if (packet.isPacketOfType(pcpp::SIP))
        sipCount++;
    if (packet.isPacketOfType(pcpp::SSL))
        sslTslCount++;
    if (packet.isPacketOfType(pcpp::BGP))
        bgpCount++;
    if (packet.isPacketOfType(pcpp::DHCP))
        dhcpv4Count++;
    if (packet.isPacketOfType(pcpp::DHCPv6))
        dhcpv6Count++;
    if (packet.isPacketOfType(pcpp::DNS))
        dnsCount++;
    if (packet.isPacketOfType(pcpp::FTP))
        ftpCount++;
    if (packet.isPacketOfType(pcpp::HTTP))
        httpCount++;
    if (packet.isPacketOfType(pcpp::SMTP))
        smtpCount++;
    if (packet.isPacketOfType(pcpp::SSH))
        sshCount++;
    if (packet.isPacketOfType(pcpp::Telnet))
        telnetCount++;
}