//
// Created by tomasz on 12/15/24.
//

#include <gtest/gtest.h>

#include "controllers/DeviceController.h"
#include "models/DeviceModel.h"
#include "views/DeviceView.h"


TEST(DeviceTest, SetDeviceNameTest) {
    auto view = std::make_shared<DeviceView>();
    auto model = std::make_shared<DeviceModel>();
    auto listener = std::make_shared<Listener>();
    auto controller = std::make_shared<DeviceController>(model, view, listener);

    controller->setNewDeviceByName("enp5s0");
    EXPECT_EQ(controller->getCurrentDeviceName(), "enp5s0");
}

TEST(DeviceTest, SetDeviceOpened) {
    auto view = std::make_shared<DeviceView>();
    auto model = std::make_shared<DeviceModel>();
    auto listener = std::make_shared<Listener>();
    auto controller = std::make_shared<DeviceController>(model, view, listener);
    controller->setIsDeviceOpen(true);
    EXPECT_EQ(controller->isDeviceOpen(), true);
}
