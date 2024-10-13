//
// Created by tomasz on 9/1/24.
//

#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include "models/DeviceModel.h"
#include "views/DeviceView.h"
#include "listeners/Listener.h"
#include "controllers/MainController.h"
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
        bool isDeviceOpen();
        void setIsDeviceOpen(bool _isDeviceOpen);
        std::vector<pcpp::PcapLiveDevice *> &getDevices();

private:

        bool m_isDeviceOpen{false};
        std::shared_ptr<DeviceModel> deviceModel;
        std::shared_ptr<DeviceView> deviceView;
        std::shared_ptr<Listener> listener;

};



#endif //DEVICECONTROLLER_H
