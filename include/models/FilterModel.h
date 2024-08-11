//
// Created by tomasz on 8/10/24.
//

#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include "PcapFilter.h"

#include "models/Model.h"

#include <vector>
#include <string>
#include <utility>
#include <atomic>

class FilterModel : public Model {
public:
    FilterModel();

    void addIpFilter(pcpp::IPAddress, pcpp::IPFilter);
    void addMacFilter(pcpp::MacAddress, pcpp::MacAddressFilter);
    void addPortFilter(int, pcpp::PortFilter);
    void addEtherTypeFilter(pcpp::ProtocolType, pcpp::EtherTypeFilter);

    std::vector<std::pair<pcpp::IPAddress, pcpp::IPFilter>>& getIpFilterVector() { return this->ipFilter; }
    std::vector<std::pair<pcpp::MacAddress, pcpp::MacAddressFilter>>& getMacFilterVector() { return this->macFilter; }
    std::vector<std::pair<int, pcpp::PortFilter>>& getPortFilterVector() { return this->portFilter; }
    std::vector<std::pair<pcpp::ProtocolType, pcpp::EtherTypeFilter>>& getEtherTypeFilterVector() { return this->etherTypeFilter; }

private:

    std::vector<std::pair<pcpp::IPAddress, pcpp::IPFilter>> ipFilter;
    std::vector<std::pair<pcpp::MacAddress, pcpp::MacAddressFilter>> macFilter;
    std::vector<std::pair<int, pcpp::PortFilter>> portFilter;
    std::vector<std::pair<pcpp::ProtocolType, pcpp::EtherTypeFilter>> etherTypeFilter;
    std::vector<pcpp::ArpFilter> arpFilter;
    std::vector<pcpp::ProtoFilter> transportFilter;
    //pcpp::TcpFlagsFilter tcpFlagsFilter;

};

#endif //FILTERMODEL_H
