#ifndef LISTENER_H
#define LISTENER_H

#include "controllers/MainController.h"
#include "models/Model.h"
#include "models/PacketCaptureModel.h"

#include <PcapFileDevice.h>
#include <PcapFilter.h>
#include <PcapLiveDevice.h>

#include <memory>
#include <thread>
#include <vector>

class MainController;
class PacketCaptureModel;

class Listener {
public:
  Listener() {}
  Listener(std::shared_ptr<PacketCaptureModel> model);

  void openListener();
  void closeListener();
  void startCapturePackets();
  void stopCapturePackets();
  void setFilters(pcpp::AndFilter);
  void setFilters(pcpp::OrFilter);
  pcpp::PcapLiveDevice *getDevice() { return this->dev; }
  std::vector<pcpp::PcapLiveDevice *> &getDevices() { return this->devices; }
  void setDeviceByName(std::string name);

  bool isDeviceCapturing();
  bool isDeviceOpen();
  bool isDeviceSelected();

  ~Listener() {};

private:
  std::shared_ptr<PacketCaptureModel> model;

  pcpp::PcapLiveDevice *dev{nullptr};

  std::vector<pcpp::PcapLiveDevice *> devices{nullptr};
  std::unique_ptr<std::thread> listener_thread;
};

#endif
