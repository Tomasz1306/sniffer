#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "controllers/FilterController.h"
#include "controllers/SearchController.h"
#include "controllers/DeviceController.h"
#include "controllers/AnalyzerController.h"
#include "controllers/StatisticController.h"
#include "controllers/DataBaseController.h"
#include "models/PacketCaptureModel.h"
#include "views/PacketCaptureView.h"
#include "views/PacketView.h"
#include "listeners/Listener.h"

#include "PcapLiveDevice.h"

#include <memory>

class SearchController;
class AnalyzerController;
class FilterController;
class DataBaseController;
class DeviceController;
class StatisticController;
class PacketCaptureModel;
class PacketCaptureView;
class PacketView;
class Listener;

class MainController : public std::enable_shared_from_this<MainController> {
public:
    MainController(std::shared_ptr<PacketCaptureModel> _model,
        std::shared_ptr<PacketCaptureView> _view,
        std::shared_ptr<Listener> _listener,
        std::shared_ptr<PacketView> _packetView,
        std::shared_ptr<AnalyzerController> _analyzerController,
        std::shared_ptr<DataBaseController> _dataBaseController,
        std::shared_ptr<FilterController> _filterController,
        std::shared_ptr<StatisticController> _statisticController,
        std::shared_ptr<DeviceController> _deviceController,
        std::shared_ptr<SearchController> _searchController);
    void display();

    static void onPacketArrivesBlockingMode(pcpp::RawPacket *packet, pcpp::PcapLiveDevice *dev, void *cookie);

    void startCapture();
    void stopCapture();
    void clearTableOfPackets();
    void setIsDisplayedPakcet(bool _isDisplayedPakcet);
    bool isDisplayedPakcet();
    bool isCapturing();
    void setCurrentPacketId(int _id);
    void displayNextPacket();
    void displayPrevPacket();
    bool isDeviceOpen();
    int getPacketCapturedVectorSize();
    void addPacketToStatistics(pcpp::Packet &packet);
    bool isDeviceSelected();
    std::string getDeviceName();
    int getDeviceId(std::string name);
    bool *getIsPacketDisplayed();
    std::string getSessionData();
    std::vector<CapturedPackets> &getCapturedVectorData();
    std::vector<CapturedPackets> getCapturedVectorDataCopy();
    std::vector<CapturedPackets> &getCapturedVectorToAnalyze();

    std::vector<CapturedPackets> &getCapturedPacketVectorDatabase();

    std::shared_ptr<DeviceController> getDeviceController();
    std::shared_ptr<MainController> getSharedPtr() { return shared_from_this(); }

private:

    bool isPacketDisplayed{false};

    std::shared_ptr<PacketCaptureView> view;
    std::shared_ptr<PacketCaptureModel> model;
    std::shared_ptr<Listener> listener;
    std::shared_ptr<PacketView> packetView;
    std::shared_ptr<StatisticController> statisticController;
    std::shared_ptr<DeviceController> deviceController;
    std::shared_ptr<AnalyzerController> analyzerController;
    std::shared_ptr<DataBaseController> databaseController;
    std::shared_ptr<FilterController> filterController;
    std::shared_ptr<SearchController> searchController;
};

#endif // MAINCONTROLLER_H
