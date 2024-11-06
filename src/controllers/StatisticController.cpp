//
// Created by tomasz on 01.10.24.
//

#include "controllers/StatisticController.h"

StatisticController::StatisticController(std::shared_ptr<StatisticModel> model, std::shared_ptr<StatisticView> view, std::shared_ptr<Listener> listener) {
    this->view = view;
    this->model = model;
    this->listener = listener;
}

std::shared_ptr<StatisticController> StatisticController::getController() {
    return shared_from_this();
}

void StatisticController::display() {
    this->view->draw(this->getController());
}

void StatisticController::addPacketToStatistics(pcpp::Packet &packet) {
    this->model->addPacketToStatistics(packet);
}

std::shared_ptr<StatisticModel> StatisticController::getModel() {
    return this->model;
}

bool StatisticController::isDeviceSelected() {
    return this->listener->isDeviceSelected();
}

void StatisticController::updateData() {
    addDataPoint(ethernetData, model->ethernetCount);
    addDataPoint(arpData, model->arpCount);
    addDataPoint(imcpv4Data, model->imcpv4Count);
    addDataPoint(ipv4Data, model->ipv4Count);
    addDataPoint(ipv6Data, model->ipv6Count);
    addDataPoint(tcpData, model->tcpCount);
    addDataPoint(udpData, model->udpCount);
    addDataPoint(dhcpv4Data, model->dhcpv4Count);
    addDataPoint(dnsData, model->dnsCount);
    addDataPoint(ftpData, model->ftpCount);
    addDataPoint(httpData, model->httpCount);
    addDataPoint(sshData, model->sshCount);
    addDataPoint(telnetData, model->telnetCount);
}

void StatisticController::addDataPoint(std::deque<int>& dataDeque, int dataPoint) {
    if (dataDeque.size() >= maxDataPoints) {
        dataDeque.pop_front();
    }
    dataDeque.push_back(dataPoint);
}

std::vector<float> StatisticController::getDequeAsVector(std::deque<int>& dataDeque) {
    return std::vector<float>(dataDeque.begin(), dataDeque.end());
}

int StatisticController::getMaxDataPoints() {
    return maxDataPoints;
}

void StatisticController::clearStatistics() {
    this->model->ethernetCount = 0;
    this->model->arpCount = 0;
    this->model->imcpv4Count = 0;
    this->model->ipv4Count = 0;
    this->model->ipv6Count = 0;
    this->model->tcpCount = 0;
    this->model->udpCount = 0;
    this->model->sslTslCount = 0;
    this->model->dhcpv4Count = 0;
    this->model->dnsCount = 0;
    this->model->ftpCount = 0;
    this->model->httpCount = 0;
    this->model->sshCount = 0;
    this->model->telnetCount = 0;
}
