//
// Created by tomasz on 9/1/24.
//

#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include "controllers/MainController.h"
#include "models/DeviceModel.h"
#include "views/DeviceView.h"
#include "listeners/Listener.h"

#include <memory>

class MainController;
class DeviceModel;
class DeviceView;
class Listener;

class DeviceController : public std::enable_shared_from_this<DeviceController> {
public:

        DeviceController(std::shared_ptr<DeviceModel> _deviceModel, std::shared_ptr<DeviceView> _deviceView, std::shared_ptr<Listener> _listener);

        void display();
        std::shared_ptr<DeviceController> getController() { return shared_from_this(); };
        std::string getCurrentDeviceName();
        void setNewDeviceByName(std::string);
        void openCurrentDevice();
        void closeCurrentDevice();
        int getDeviceId(std::string name);
        bool isDeviceOpen();
        bool isCapturing();
        void setIsDeviceOpen(bool _isDeviceOpen);
        void setMainController(std::shared_ptr<MainController> controller) {this->mainController = controller;}
        std::vector<pcpp::PcapLiveDevice *> &getDevices();

private:

        bool m_isDeviceOpen{false};
        std::shared_ptr<DeviceModel> deviceModel;
        std::shared_ptr<DeviceView> deviceView;
        std::shared_ptr<Listener> listener;
        std::shared_ptr<MainController> mainController;

};

#endif //DEVICECONTROLLER_H
