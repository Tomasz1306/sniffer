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

void StatisticController::updateData(const std::string& timestamp) {
    std::tm t{};
    std::istringstream ss(timestamp);

    ss >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
        throw std::runtime_error{"failed to parse time string"};
    }

    std::time_t epochTime = mktime(&t);
    double numericTimestamp = static_cast<double>(epochTime);

    if (firstMeasurement) {
        startTime = numericTimestamp;
        firstMeasurement = false;
    }

    double relativeTime = numericTimestamp - startTime;
    addDataPoint(ethernetData, model->ethernetCount, relativeTime);
    addDataPoint(arpData, model->arpCount, relativeTime);
    addDataPoint(imcpv4Data, model->imcpv4Count, relativeTime);
    addDataPoint(ipv4Data, model->ipv4Count, relativeTime);
    addDataPoint(ipv6Data, model->ipv6Count, relativeTime);
    addDataPoint(tcpData, model->tcpCount, relativeTime);
    addDataPoint(udpData, model->udpCount, relativeTime);
    addDataPoint(dhcpv4Data, model->dhcpv4Count, relativeTime);
    addDataPoint(dnsData, model->dnsCount, relativeTime);
    addDataPoint(ftpData, model->ftpCount, relativeTime);
    addDataPoint(httpData, model->httpCount, relativeTime);
    addDataPoint(sshData, model->sshCount, relativeTime);
    addDataPoint(telnetData, model->telnetCount, relativeTime);
}

void StatisticController::addDataPoint(std::deque<std::pair<double, int>>& dataDeque, int dataPoint, double time) {
    if (dataDeque.size() >= maxDataPoints) {
        dataDeque.pop_front();
    }
    dataDeque.push_back({time, dataPoint});
}

std::vector<std::pair<float, float>> StatisticController::getDequeAsVector(std::deque<std::pair<double, int>>& dataDeque) {
    std::vector<std::pair<float, float>> vec;
    vec.reserve(dataDeque.size());
    for (const auto& p : dataDeque) {
        vec.push_back({static_cast<float>(p.first), static_cast<float>(p.second)});
    }
    return vec;
}

int StatisticController::getMaxDataPoints() {
    return maxDataPoints;
}

void StatisticController::clearStatistics() {
    // this->model->ethernetCount.first = 0;
    // this->model->ethernetCount.second = "";
    // this->model->arpCount.first = 0;
    // this->model->arpCount.second = "";
    // this->model->imcpv4Count.first = 0;
    // this->model->imcpv4Count.second = "";
    // this->model->ipv4Count.first = 0;
    // this->model->ipv4Count.second = "";
    // this->model->ipv6Count.first = 0;
    // this->model->ipv6Count.second = "";
    // this->model->tcpCount.first = 0;
    // this->model->tcpCount.second = "";
    // this->model->udpCount.first = 0;
    // this->model->udpCount.second = "";
    // this->model->ftpCount.first = 0;
    // this->model->ftpCount.second = "";
    // this->model->httpCount.first = 0;
    // this->model->httpCount.second = "";
    // this->model->sshCount.first = 0;
    // this->model->sshCount.second = "";
    // this->model->telnetCount.first = 0;
    // this->model->telnetCount.second = "";
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
