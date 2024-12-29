#include "listeners/Listener.h"
#include "controllers/LogController.h"
#include "utils/Utils.h"

#include <PcapLiveDevice.h>
#include <PcapLiveDeviceList.h>
#include <chrono>
#include <ctime>
#include <iostream>

Listener::Listener(std::shared_ptr<PacketCaptureModel> model) {
  this->devices =
      pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDevicesList();
  this->dev = nullptr;
  this->model = model;
}

void Listener::openListener() {
  if (this->dev == nullptr) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS NOT SELECTED", LogType::ERROR);
    return;
  }
  if (this->dev->isOpened()) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS ACTUALLY OPEN", LogType::ERROR);
    return;
  }
  if (this->dev->open()) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "DEVICE IS OPENED AT INTERFACE", LogType::SUCCESFULL);
  } else {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR CANT OPEN DEVICE", LogType::ERROR);
  }
}

void Listener::closeListener() {
  if (this->dev == nullptr) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS NOT SELECTED", LogType::ERROR);
    // WRITE TO LOG
    return;
  }
  if (this->dev->captureActive()) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS CAPTURING", LogType::WARNING);
    return;
  }
  if (!this->dev->isOpened()) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS NOT OPEN", LogType::ERROR);
    // WRITE TO LOG
    return;
  }
  LogController::getInstance()->addLog(Utils::getTime(), "DEVICE IS CLOSED",
                                       LogType::SUCCESFULL);
  this->dev->close();
}

void Listener::startCapturePackets() {
  if (this->dev == nullptr) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS NOT SELECTED", LogType::ERROR);
    // WRITE TO LOG
    return;
  }
  if (!this->dev->isOpened()) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS NOT OPEN", LogType::ERROR);
    // WRITE TO LOG
    return;
  }
  LogController::getInstance()->addLog(
      Utils::getTime(), "START CAPTURING PACKETS", LogType::SUCCESFULL);
  this->dev->startCapture(MainController::onPacketArrivesBlockingMode, &model);
}

void Listener::stopCapturePackets() {
  if (this->dev == nullptr) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS NOT SELECTED", LogType::ERROR);
    // WRITE TO LOG
    return;
  }
  if (!this->dev->isOpened()) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS NOT OPEN", LogType::ERROR);
    // WRITE TO LOG
    return;
  }
  if (!this->dev->captureActive()) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS NOT CAPTURING", LogType::ERROR);
    // WRITE TO LOG
    return;
  }
  LogController::getInstance()->addLog(
      Utils::getTime(), "STOP CAPTURING PACKETS", LogType::SUCCESFULL);
  this->dev->stopCapture();
}

void Listener::setFilters(pcpp::AndFilter filter) {
  if (this->dev == nullptr) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS NOT SELECTED", LogType::ERROR);
    return;
  }
  if (this->dev->captureActive()) {
    this->stopCapturePackets();
    this->dev->setFilter(filter);
    this->startCapturePackets();
  }
  if (!this->dev->captureActive()) {
    this->dev->setFilter(filter);
  }
}

void Listener::setFilters(pcpp::OrFilter filter) {
  if (this->dev == nullptr) {
    LogController::getInstance()->addLog(
        Utils::getTime(), "ERROR DEVICE IS NOT SELECTED", LogType::ERROR);
    return;
  }
  if (this->dev->captureActive()) {
    this->stopCapturePackets();
    this->dev->setFilter(filter);
    this->startCapturePackets();
  }
  if (!this->dev->captureActive()) {
    this->dev->setFilter(filter);
  }
}

void Listener::setDeviceByName(std::string name) {
  if (this->dev != nullptr) {
    if (this->dev->isOpened()) {
      LogController::getInstance()->addLog(Utils::getTime(), "DEVICE IS OPEN ",
                                           LogType::SUCCESFULL);
      this->dev->close();
      this->dev = nullptr;
    }

    if (this->dev->captureActive()) {
      LogController::getInstance()->addLog(
          Utils::getTime(), "DEVICE IS CAPTURING", LogType::SUCCESFULL);
      return;
    }
  }
  this->dev =
      pcpp::PcapLiveDeviceList::getInstance().getPcapLiveDeviceByName(name);

  if (this->dev != nullptr) {
    LogController::getInstance()->addLog(Utils::getTime(), "DEVICE IS SELECTED",
                                         LogType::SUCCESFULL);
  }
}

bool Listener::isDeviceOpen() {
  if (this->dev == nullptr) {
    return false;
  }
  return this->dev->isOpened();
}

bool Listener::isDeviceSelected() {
  if (this->dev == nullptr) {
    return false;
  } else {
    return true;
  }
}

bool Listener::isDeviceCapturing() { return this->dev->captureActive(); }
