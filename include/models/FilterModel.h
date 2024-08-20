//
// Created by tomasz on 8/10/24.
//

#ifndef FILTERMODEL_H
#define FILTERMODEL_H

#include "PcapFilter.h"

#include "models/Model.h"
#include "global/Global.h"

#include <vector>
#include <string>
#include <utility>
#include <atomic>
#include <mutex>

class FilterModel : public Model {
public:
    FilterModel();

    void setUpdate(bool);
    void addIpFilter(pcpp::IPAddress, pcpp::IPFilter);
    void addMacFilter(pcpp::MacAddress, pcpp::MacAddressFilter);
    void addPortFilter(int, pcpp::PortFilter);
    void addEtherTypeFilter(std::string, std::shared_ptr<bool>);

    std::vector<std::pair<pcpp::IPAddress, pcpp::IPFilter>>& getIpFilterVector() { return this->ipFilter; }
    std::vector<std::pair<pcpp::MacAddress, pcpp::MacAddressFilter>>& getMacFilterVector() { return this->macFilter; }
    std::vector<std::pair<int, pcpp::PortFilter>>& getPortFilterVector() { return this->portFilter; }
    std::vector<std::pair<std::string, std::shared_ptr<bool>>>& getEtherTypeFilterVector() { return this->etherTypeFilter; }
    std::vector<std::pair<std::string, std::shared_ptr<bool>>>& getTcpFlagsVector() { return this->tcpFlagsFilter; }
    bool getUpdate() {return this->update.load(std::memory_order_relaxed); };

private:

    std::atomic_bool update{false};
    std::string filters;

    std::vector<std::pair<pcpp::IPAddress, pcpp::IPFilter>> ipFilter;
    std::vector<std::pair<pcpp::MacAddress, pcpp::MacAddressFilter>> macFilter;
    std::vector<std::pair<int, pcpp::PortFilter>> portFilter;
    std::vector<std::pair<std::string, std::shared_ptr<bool>>> etherTypeFilter;
    std::vector<pcpp::ArpFilter> arpFilter;
    std::vector<pcpp::ProtoFilter> transportFilter;
    std::vector<std::pair<std::string, std::shared_ptr<bool>>> tcpFlagsFilter;

};

#endif //FILTERMODEL_H
