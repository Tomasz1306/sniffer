//
// Created by tomasz on 8/10/24.
//
#include "models/FilterModel.h"

FilterModel::FilterModel() {
    this->tcpFlagsFilter.emplace_back("FIN", std::make_shared<bool>(true));
    this->tcpFlagsFilter.emplace_back("SYN", std::make_shared<bool>(true));
    this->tcpFlagsFilter.emplace_back("RST", std::make_shared<bool>(true));
    this->tcpFlagsFilter.emplace_back("PSH", std::make_shared<bool>(true));
    this->tcpFlagsFilter.emplace_back("ACK", std::make_shared<bool>(true));
    this->tcpFlagsFilter.emplace_back("URG", std::make_shared<bool>(true));

    this->etherTypeFilter.emplace_back("TCP", std::make_shared<bool>(true));
    this->etherTypeFilter.emplace_back("UDP", std::make_shared<bool>(true));
    this->etherTypeFilter.emplace_back("ICMP", std::make_shared<bool>(true));
    this->etherTypeFilter.emplace_back("VLAN", std::make_shared<bool>(true));
    this->etherTypeFilter.emplace_back("IPv4", std::make_shared<bool>(true));
    this->etherTypeFilter.emplace_back("IPv6", std::make_shared<bool>(true));
    this->etherTypeFilter.emplace_back("ARP", std::make_shared<bool>(true));
    this->etherTypeFilter.emplace_back("ETH", std::make_shared<bool>(true));

}

void FilterModel::setUpdate(bool _update) {
    this->update = _update;
}


void FilterModel::addIpFilter(pcpp::IPAddress _ipAddress, pcpp::IPFilter  _ipFilter) {
    this->ipFilter.emplace_back(_ipAddress, _ipFilter);
}

void FilterModel::addMacFilter(pcpp::MacAddress _macAddress, pcpp::MacAddressFilter  _macFilter) {
    this->macFilter.emplace_back(_macAddress, _macFilter);
}

void FilterModel::addPortFilter(int _port, pcpp::PortFilter  _portFilter) {
    this->portFilter.emplace_back(_port, _portFilter);
}

void FilterModel::addEtherTypeFilter(std::string _protocolType, std::shared_ptr<bool>  _etherTypeFilter) {
    this->etherTypeFilter.emplace_back(_protocolType, _etherTypeFilter);
}

