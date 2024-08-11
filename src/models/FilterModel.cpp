//
// Created by tomasz on 8/10/24.
//
#include "models/FilterModel.h"

FilterModel::FilterModel() {

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

void FilterModel::addEtherTypeFilter(pcpp::ProtocolType _protocolType, pcpp::EtherTypeFilter  _etherTypeFilter) {
    this->etherTypeFilter.emplace_back(_protocolType, _etherTypeFilter);
}

