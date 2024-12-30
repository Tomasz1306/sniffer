//
// Created by tomasz on 12/15/24.
//

#include <gtest/gtest.h>

#include "controllers/LogController.h"
#include "models/LogModel.h"
#include "views/LogView.h"

TEST(LogTest, InstanceTest) {
    auto instance1 = LogController::getInstance();
    auto instance2 = LogController::getInstance();
    EXPECT_EQ(instance1, instance2);
}

TEST(LogTest, AddLogTest) {
    LogModel model1(""), model2(""), model3("");
    std::shared_ptr<LogModel> modelPtr1 = std::make_shared<LogModel>("");
    model1.addLog("01-01-2024 12:00:00", "test success", LogType::SUCCESFULL);
    model2.addLog("01-01-2024 12:00:00", "test warning", LogType::WARNING);
    model3.addLog("01-01-2024 12:00:00", "test error", LogType::ERROR);
    auto controller = LogController::getInstance();
    controller->setModel(modelPtr1);
    controller->addLog("01-01-2024 12:00:00", "test success", LogType::SUCCESFULL);
    controller->addLog("01-01-2024 12:00:00", "test warning", LogType::WARNING);
    controller->addLog("01-01-2024 12:00:00", "test error", LogType::ERROR);
    EXPECT_EQ(controller->getLogs()[0], model1.getLogs()[0]);
    EXPECT_EQ(controller->getLogs()[1], model2.getLogs()[0]);
    EXPECT_EQ(controller->getLogs()[2], model3.getLogs()[0]);
}