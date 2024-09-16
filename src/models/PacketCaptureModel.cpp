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
            this->capturedPackets_deque.pop_front();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

void PacketCaptureModel::clearCapturedPacketVector() {
    std::lock_guard lock(guard_1);
    this->capturedPackets_vector.clear();
}

PacketCaptureModel::~PacketCaptureModel() {
    if (this->thread_1->joinable()) {
        this->thread_1->join();
    }
}
