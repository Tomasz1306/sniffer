//
// Created by tomasz on 01.10.24.
//

#ifndef STATISTICCONTROLLER_H
#define STATISTICCONTROLLER_H
#include <memory>

#include "models/StatisticModel.h"
#include "listeners/Listener.h"
#include "views/StatisticView.h"
#include "Packet.h"
#include <map>

class StatisticModel;
class StatisticView;
class Listener;

class StatisticController : public std::enable_shared_from_this<StatisticController> {
public:
    StatisticController(std::shared_ptr<StatisticModel> model, std::shared_ptr<StatisticView> view, std::shared_ptr<Listener> listener);
    std::shared_ptr<StatisticController> getController();
    void display();
    void updateData();
    void addDataPoint(std::deque<int>& dataDeque, int dataPoint);
    std::vector<float> getDequeAsVector(std::deque<int>& dataDeque);

    void addPacketToStatistics(pcpp::Packet& packet);
    std::shared_ptr<StatisticModel> getModel();
    bool isDeviceSelected();

    std::vector<float> getEthernetData() { return getDequeAsVector(ethernetData); }
    std::vector<float> getArpData() { return getDequeAsVector(arpData); }
    std::vector<float> getImcpv4Data() { return getDequeAsVector(imcpv4Data); }
    std::vector<float> getIpv4Data() { return getDequeAsVector(ipv4Data); }
    std::vector<float> getIpv6Data() { return getDequeAsVector(ipv6Data); }
    std::vector<float> getTcpData() { return getDequeAsVector(tcpData); }
    std::vector<float> getUdpData() { return getDequeAsVector(udpData); }
    std::vector<float> getDhcpv4Data() { return getDequeAsVector(dhcpv4Data); }
    std::vector<float> getDnsData() { return getDequeAsVector(dnsData); }
    std::vector<float> getFtpData() { return getDequeAsVector(ftpData); }
    std::vector<float> getHttpData() { return getDequeAsVector(httpData); }
    std::vector<float> getSshData() { return getDequeAsVector(sshData); }
    std::vector<float> getTelnetData() { return getDequeAsVector(telnetData); }
    std::vector<float> getSmtpData() { return getDequeAsVector(smtpData); }

    int getMaxDataPoints();
    void clearStatistics();

private:
    std::shared_ptr<StatisticModel> model;
    std::shared_ptr<StatisticView> view;
    std::shared_ptr<Listener> listener;
    std::deque<int> ethernetData;
    std::deque<int> arpData;
    std::deque<int> imcpv4Data;
    std::deque<int> ipv4Data;
    std::deque<int> ipv6Data;
    std::deque<int> tcpData;
    std::deque<int> udpData;
    std::deque<int> dhcpv4Data;
    std::deque<int> dnsData;
    std::deque<int> ftpData;
    std::deque<int> httpData;
    std::deque<int> sshData;
    std::deque<int> telnetData;
    std::deque<int> smtpData;
    const int maxDataPoints = 10000;

};

#endif //STATISTICCONTROLLER_H
