//
// Created by tomasz on 9/1/24.
//

#ifndef DEVICECONTROLLER_H
#define DEVICECONTROLLER_H

#include "models/DeviceModel.h"
#include "views/DeviceView.h"
#include "listeners/Listener.h"
#include <memory>

class DeviceModel;
class DeviceView;

class DeviceController : public std::enable_shared_from_this<DeviceController> {
public:

        DeviceController(std::shared_ptr<DeviceModel> _deviceModel, std::shared_ptr<DeviceView> _deviceView, std::shared_ptr<Listener> _listener);

        void display();
        std::shared_ptr<DeviceController> getController() { return shared_from_this(); };

private:

        std::shared_ptr<DeviceModel> deviceModel;
        std::shared_ptr<DeviceView> deviceView;
        std::shared_ptr<Listener> listener;

};



#endif //DEVICECONTROLLER_H
