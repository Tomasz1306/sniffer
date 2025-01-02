//
// Created by tomasz on 8/10/24.
//
#include "controllers/FilterController.h"
#include "HttpLayer.h"
#include "PcapFilter.h"
#include "controllers/LogController.h"
#include "utils/Utils.h"
#include <iostream>

FilterController::FilterController(std::shared_ptr<FilterModel> _model,
                                   std::shared_ptr<FilterView> _view,
                                   std::shared_ptr<Listener> _listener) {
  this->thread_update =
      std::make_shared<std::thread>(&FilterController::update, this);
  this->model = _model;
  this->view = _view;
  this->listener = _listener;
}
void FilterController::run() {}

void FilterController::update() {
  while (this->isUpdateRunning) {
    if (this->model != nullptr && this->model->getUpdate()) {
      pcpp::OrFilter orFilter;
      pcpp::OrFilter applicationLayerOrFilter;
      pcpp::AndFilter andFilter;
      pcpp::OrFilter orFilterIpAddress;
      pcpp::OrFilter orFilterMacAddress;
      pcpp::OrFilter portOrFilter;
      pcpp::ProtoFilter tcpFilter(pcpp::TCP), udpFilter(pcpp::UDP),
          icmpFilter(pcpp::ICMP), ivp4Filter(pcpp::IPv4),
          ipv6Filter(pcpp::IPv6), arpFilter(pcpp::ARP), httpFilter(pcpp::HTTP),
          sshFilter(pcpp::SSH), telnetFilter(pcpp::Telnet),
          ftpFilter(pcpp::FTP), icmpv6Filter(pcpp::ICMPv6);
      uint8_t tcpFlags = 0;
      pcpp::AndFilter tcpAndFilter;
      bool isTcp{false}, isIpOrMacAddress{false}, isPortSelected{false};
      pcpp::TcpFlagsFilter globalTcpFlagsFilter(
          0, pcpp::TcpFlagsFilter::MatchOneAtLeast);
      pcpp::PortFilter portFilter(80, pcpp::SRC_OR_DST);

      for (auto &ethType : this->model->getEtherTypeFilterVector()) {
        if (*ethType.second) {
          if (ethType.first == "TCP") {
            for (auto &tcpFlag : this->model->getTcpFlagsVector()) {
              if (tcpFlag.first == "FIN") {
                if (*tcpFlag.second) {
                  tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpFin;
                }
              } else if (tcpFlag.first == "SYN") {
                if (*tcpFlag.second) {
                  tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpSyn;
                }
              } else if (tcpFlag.first == "ACK") {
                if (*tcpFlag.second) {
                  tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpAck;
                }
              } else if (tcpFlag.first == "PSH") {
                if (*tcpFlag.second) {
                  tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpPush;
                }
              } else if (tcpFlag.first == "RST") {
                if (*tcpFlag.second) {
                  tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpRst;
                }
              } else if (tcpFlag.first == "URG") {
                if (*tcpFlag.second) {
                  tcpFlags |= pcpp::TcpFlagsFilter::TcpFlags::tcpUrg;
                }
              }
            }
            pcpp::TcpFlagsFilter tcpFlagsFilter(
                tcpFlags, pcpp::TcpFlagsFilter::MatchOneAtLeast);
            globalTcpFlagsFilter = tcpFlagsFilter;
            tcpAndFilter.addFilter(&tcpFilter);
            tcpAndFilter.addFilter(&globalTcpFlagsFilter);
            orFilter.addFilter(&tcpAndFilter);
          }
          if (ethType.first == "UDP") {
            orFilter.addFilter(&udpFilter);
          }
          if (ethType.first == "ICMP") {
            orFilter.addFilter(&icmpFilter);
          }
          if (ethType.first == "IPv4") {
            orFilter.addFilter(&ivp4Filter);
          }
          if (ethType.first == "IPv6") {
            orFilter.addFilter(&ipv6Filter);
          }
          if (ethType.first == "ARP") {
            orFilter.addFilter(&arpFilter);
          }
          if (ethType.first == "ICMPv6") {
            orFilter.addFilter(&icmpv6Filter);
          }
        }
      }
      if (!this->model->getPortFilterVector().empty()) {
        for (auto &port : this->model->getPortFilterVector()) {
          portOrFilter.addFilter(&port.second);
        }
        isPortSelected = true;
        andFilter.addFilter(&portOrFilter);
      }

      if (!this->model->getIpFilterVector().empty()) {
        for (auto &ip : this->model->getIpFilterVector()) {
          orFilterIpAddress.addFilter(&ip.second);
        }
        isIpOrMacAddress = true;
        andFilter.addFilter(&orFilterIpAddress);
      }
      if (!this->model->getMacFilterVector().empty()) {
        for (auto &mac : this->model->getMacFilterVector()) {
          orFilterMacAddress.addFilter(&mac.second);
        }
        isIpOrMacAddress = true;
        andFilter.addFilter(&orFilterMacAddress);
      }
      if (isIpOrMacAddress || isPortSelected) {
        andFilter.addFilter(&orFilter);
        std::string filterString;
        andFilter.parseToString(filterString);

        this->actualBfsFilter = filterString;
        this->listener->setFilters(andFilter);
        LogController::getInstance()->addLog(Utils::getTime(), filterString,
                                             LogType::SUCCESFULL);
        this->model->setUpdate(false);
      } else {
        std::string filterString;
        orFilter.parseToString(filterString);
        this->actualBfsFilter = filterString;
        this->listener->setFilters(orFilter);
        LogController::getInstance()->addLog(Utils::getTime(), filterString,
                                             LogType::SUCCESFULL);
        this->model->setUpdate(false);
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

std::string FilterController::getFilters() { return this->actualBfsFilter; }

void FilterController::display() {
  this->view->draw(shared_from_this(), this->model);
}

void FilterController::setMainController(
    std::shared_ptr<MainController> controller) {
  this->mainController = controller;
}

bool FilterController::isDeviceOpen() {
  if (this->mainController == nullptr) {
    return false;
  }
  return this->mainController->isDeviceOpen();
}

void FilterController::setIsUpdateRunning(bool status) {
  this->isUpdateRunning = status;
}

FilterController::~FilterController() {
  if (this->thread_update->joinable()) {
    this->thread_update->join();
  }
}
