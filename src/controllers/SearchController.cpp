//
// Created by tomasz on 11/12/24.
//

#include "controllers/SearchController.h"


SearchController::SearchController(std::shared_ptr<SearchModel> model, std::shared_ptr<SearchView> view) {
    this->model = model;
    this->view = view;
}

void SearchController::display() {
    this->view->draw(shared_from_this());
}

std::string SearchController::byteToHex(unsigned char byte) {
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << (int)byte;
    return ss.str();
}

void SearchController::setMainController(std::shared_ptr<MainController> controller) {
    this->mainController = controller;
}

void SearchController::search(std::string key) {
    this->findedPackets.clear();
    std::vector<CapturedPackets> vectorToSearch{mainController->getCapturedVectorData()};
    for (const auto &packet: vectorToSearch) {
        std::vector<std::string> layerList;
        std::stringstream asciiStream;
        packet.packet.toStringList(layerList);
        for (const auto &layer: layerList) {
            if (std::string::npos != layer.find(key)) {
                this->findedPackets.push_back(packet);
                break;
            }
            pcpp::PayloadLayer* payloadLayer = packet.packet.getLayerOfType<pcpp::PayloadLayer>();
            if (payloadLayer !=nullptr) {
                const uint8_t *payload = packet.packet.getLayerOfType<pcpp::PayloadLayer>()->getPayload();
                int payloadSize = packet.packet.getLayerOfType<pcpp::PayloadLayer>()->getPayloadLen();
                if (payload) {
                    for (int i = 0; i < payloadSize; i++) {
                        unsigned char byte = payload[i];
                        std::stringstream hexStream;
                        asciiStream << (isprint(byte) ? static_cast<char>(byte) : '.');
                    }
                }
            }
            std::string asciiString = asciiStream.str();
            if (std::string::npos != asciiString.find(key)) {
                this->findedPackets.push_back(packet);
                break;
            }
        }
    }
}

std::vector<CapturedPackets>& SearchController::getFindedPackets() {
    return this->findedPackets;
}

void SearchController::displayPacket(int id) {
    mainController->setCurrentPacketId(id);
    mainController->setIsDisplayedPakcet(true);
}
