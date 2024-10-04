//
// Created by tomasz on 01.10.24.
//

#ifndef STATISTICCONTROLLER_H
#define STATISTICCONTROLLER_H
#include <memory>

#include "models/StatisticModel.h"
#include "listeners/Listener.h"
#include "views/StatisticView.h"
#include "Packet.h"

class StatisticModel;
class StatisticView;
class Listener;

class StatisticController : public std::enable_shared_from_this<StatisticController> {
public:
    StatisticController(std::shared_ptr<StatisticModel> model, std::shared_ptr<StatisticView> view, std::shared_ptr<Listener> listener);
    std::shared_ptr<StatisticController> getController();
    void display();

    void addPacketToStatistics(pcpp::Packet& packet);
    std::shared_ptr<StatisticModel> getModel();
    bool isDeviceSelected();

private:
    std::shared_ptr<StatisticModel> model;
    std::shared_ptr<StatisticView> view;
    std::shared_ptr<Listener> listener;

};

#endif //STATISTICCONTROLLER_H
