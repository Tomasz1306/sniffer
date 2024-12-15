//
// Created by tomasz on 01.10.24.
//

#ifndef STATISTICCONTROLLER_H
#define STATISTICCONTROLLER_H

#include "controllers/MainController.h"
#include "models/StatisticModel.h"
#include "views/StatisticView.h"
#include "listeners/Listener.h"

#include <memory>
#include <deque>

class MainController;
class StatisticModel;
class StatisticView;
class Listener;

class StatisticController : public std::enable_shared_from_this<StatisticController> {
public:
    StatisticController(std::shared_ptr<StatisticModel> model, std::shared_ptr<StatisticView> view, std::shared_ptr<Listener> listener);
    std::shared_ptr<StatisticController> getController();
    void display();
    void updateData(const std::string& timestamp);
    void addDataPoint(std::deque<std::pair<double, int>>& dataDeque, int dataPoint, double time);
    std::vector<std::pair<float, float>> getDequeAsVector(std::deque<std::pair<double, int>>& dataDeque);

    void addPacketToStatistics(pcpp::Packet& packet);
    std::shared_ptr<StatisticModel> getModel();
    bool isDeviceSelected();

    std::vector<std::pair<float, float>>  getEthernetData() { return getDequeAsVector(ethernetData); }
    std::vector<std::pair<float, float>>  getArpData() { return getDequeAsVector(arpData); }
    std::vector<std::pair<float, float>>  getImcpv4Data() { return getDequeAsVector(imcpv4Data); }
    std::vector<std::pair<float, float>>  getIpv4Data() { return getDequeAsVector(ipv4Data); }
    std::vector<std::pair<float, float>>  getIpv6Data() { return getDequeAsVector(ipv6Data); }
    std::vector<std::pair<float, float>>  getTcpData() { return getDequeAsVector(tcpData); }
    std::vector<std::pair<float, float>>  getUdpData() { return getDequeAsVector(udpData); }
    std::vector<std::pair<float, float>>  getDhcpv4Data() { return getDequeAsVector(dhcpv4Data); }
    std::vector<std::pair<float, float>>  getDnsData() { return getDequeAsVector(dnsData); }
    std::vector<std::pair<float, float>>  getFtpData() { return getDequeAsVector(ftpData); }
    std::vector<std::pair<float, float>>  getHttpData() { return getDequeAsVector(httpData); }
    std::vector<std::pair<float, float>>  getSshData() { return getDequeAsVector(sshData); }
    std::vector<std::pair<float, float>>  getTelnetData() { return getDequeAsVector(telnetData); }
    // std::vector<float> getSmtpData() { return getDequeAsVector(smtpData); }

    void setMainController(std::shared_ptr<MainController> controller) {this->mainController = controller;}
    int getMaxDataPoints();
    void clearStatistics();

public:
    std::shared_ptr<StatisticModel> model;
    std::shared_ptr<StatisticView> view;
    std::shared_ptr<Listener> listener;
    std::deque<std::pair<double, int>> ethernetData;
    std::deque<std::pair<double, int>> arpData;
    std::deque<std::pair<double, int>> imcpv4Data;
    std::deque<std::pair<double, int>> ipv4Data;
    std::deque<std::pair<double, int>> ipv6Data;
    std::deque<std::pair<double, int>> tcpData;
    std::deque<std::pair<double, int>> udpData;
    std::deque<std::pair<double, int>> dhcpv4Data;
    std::deque<std::pair<double, int>> dnsData;
    std::deque<std::pair<double, int>> ftpData;
    std::deque<std::pair<double, int>> httpData;
    std::deque<std::pair<double, int>> sshData;
    std::deque<std::pair<double, int>> telnetData;
    const int maxDataPoints = 10000;
    bool firstMeasurement = true;
    double startTime = 0.0;


    std::shared_ptr<MainController> mainController;
};

#endif //STATISTICCONTROLLER_H
