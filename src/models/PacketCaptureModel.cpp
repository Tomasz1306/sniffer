#include "models/PacketCaptureModel.h"

#include "controllers/LogController.h"
#include "global/Global.h"
#include "utils/Utils.h"

PacketCaptureModel::PacketCaptureModel(){
    this->thread_1 = std::make_shared<std::thread>(&PacketCaptureModel::writeFromDequeToVector, this);
}

std::vector<CapturedPackets>& PacketCaptureModel::getCapturedPacketVector(){
    return this->capturedPackets_vector;
}

void PacketCaptureModel::addToCapturedPacketDeque(pcpp::Packet packet){
    this->capturedPackets_deque.push_back(packet);
}

void PacketCaptureModel::writeFromDequeToVector(){
    while(1) {
        if (!this->capturedPackets_deque.empty()) {
            std::scoped_lock lock(guard_1, guard_3, guard_4);
            auto &packet = this->capturedPackets_deque.back();
            this->capturedPackets_vector.emplace_back(this->counter, false, "", this->capturedPackets_deque.back());
            this->capturedPackets_database.emplace_back(this->counter, false, Utils::getTime(), this->capturedPackets_deque.back());
            database_cv.notify_all();
            ++this->counter;
            this->controller->addPacketToStatistics(this->capturedPackets_vector.back().packet);
            this->capturedPackets_deque.pop_back();
        }
    }
}

void PacketCaptureModel::clearCapturedPacketVector() {
    std::lock_guard lock(guard_1);
    this->capturedPackets_vector.clear();
    this->counter = 0;
}

void PacketCaptureModel::clearCapturedPacketVectorDatabase() {
    std::lock_guard lock(guard_4);
    this->capturedPackets_database.clear();
}


PacketCaptureModel::~PacketCaptureModel() {
    if (this->thread_1->joinable()) {
        this->thread_1->join();
    }
}

void PacketCaptureModel::setMainController(std::shared_ptr<MainController> controller) {
    this->controller = controller;
}

std::vector<CapturedPackets> &PacketCaptureModel::getCapturedPacketVectorDatabase() {
    return this->capturedPackets_database;
}