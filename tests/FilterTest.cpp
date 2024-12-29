//
// Created by tomasz on 12/28/24.
//

#include <gtest/gtest.h>

#include "PcapFilter.h"
#include "controllers/FilterController.h"
#include "listeners/Listener.h"
#include "models/FilterModel.h"
#include "views/FilterView.h"

TEST(FilterTest, filterByIpAddress) {
  auto stopUpdate = [](std::shared_ptr<FilterController> controller) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    controller->setIsUpdateRunning(false);
  };
  auto filterView = std::make_shared<FilterView>();
  auto filterModel = std::make_shared<FilterModel>();
  auto listener = std::make_shared<Listener>();
  auto filterController =
      std::make_shared<FilterController>(filterModel, filterView, listener);
  std::string address("192.168.0.1");
  filterModel->getIpFilterVector().emplace_back(
      pcpp::IPAddress(address), pcpp::IPFilter(address, pcpp::SRC_OR_DST));
  filterModel->setUpdate(true);
  std::thread stopUpdateThread(stopUpdate, filterController);
  stopUpdateThread.detach();
  filterController->update();
  std::string correct =
      "((ip and src or dst net 192.168.0.1/32)) and (((tcp) and (tcp[tcpflags] "
      "& (tcp-fin|tcp-syn|tcp-rst|tcp-push|tcp-ack|tcp-urg) != 0)) or (udp) or "
      "(icmp) or (ip) or (ip6))";
  bool finded{false};
  if (correct.find("ip and src or dst net 192.168.0.1") != std::string::npos) {
    finded = true;
  }
  EXPECT_EQ(finded, true);
  if (stopUpdateThread.joinable()) {
    stopUpdateThread.join();
  }
}
TEST(FilterTest, filterByMacAddress) {
  auto stopUpdate = [](std::shared_ptr<FilterController> controller) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    controller->setIsUpdateRunning(false);
  };
  auto filterView = std::make_shared<FilterView>();
  auto filterModel = std::make_shared<FilterModel>();
  auto listener = std::make_shared<Listener>();
  auto filterController =
      std::make_shared<FilterController>(filterModel, filterView, listener);
  std::string address("00:00:00:22:22:22");
  filterModel->getMacFilterVector().emplace_back(
      pcpp::MacAddress(address),
      pcpp::MacAddressFilter(address, pcpp::SRC_OR_DST));
  filterModel->setUpdate(true);
  std::thread stopUpdateThread(stopUpdate, filterController);
  stopUpdateThread.detach();
  filterController->update();
  std::string correct =
      "((ether host 00:00:00:22:22:22)) and (((tcp) and (tcp[tcpflags] & "
      "(tcp-fin|tcp-syn|tcp-rst|tcp-push|tcp-ack|tcp-urg) != 0)) or (udp) or "
      "(icmp) or (ip) or (ip6))";
  bool finded{false};
  if (correct.find("ether host 00:00:00:22:22:22") != std::string::npos) {
    finded = true;
  }
  EXPECT_EQ(finded, true);
  if (stopUpdateThread.joinable()) {
    stopUpdateThread.join();
  }
}
