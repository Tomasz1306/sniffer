#include "models/PacketCaptureModel.h"
#include "global/Global.h"

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
            std::lock_guard lock(guard_1);
            this->capturedPackets_vector.emplace_back(this->counter, false, "", this->capturedPackets_deque.front());
            ++this->counter;
            this->controller->addPacketToStatistics(this->capturedPackets_vector.back().packet);
            this->capturedPackets_deque.pop_front();
        }
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void PacketCaptureModel::clearCapturedPacketVector() {
    std::lock_guard lock(guard_1);
    this->capturedPackets_vector.clear();
    this->counter = 0;
}

PacketCaptureModel::~PacketCaptureModel() {
    if (this->thread_1->joinable()) {
        this->thread_1->join();
    }
}

void PacketCaptureModel::setController(std::shared_ptr<MainController> controller) {
    this->controller = controller;
}
