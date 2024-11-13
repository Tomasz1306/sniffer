//
// Created by tomasz on 11/12/24.
//

#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H

#include "controllers/MainController.h"
#include "models/SearchModel.h"
#include "models/PacketModel.h"
#include "views/SearchView.h"

#include <memory>
#include <sstream>
#include <iomanip>

class MainController;
class SearchModel;
class SearchView;

class SearchController : public std::enable_shared_from_this<SearchController> {
public:
    SearchController(std::shared_ptr<SearchModel> model, std::shared_ptr<SearchView> view);

    void setMainController(std::shared_ptr<MainController> controller);
    void display();

    void search(std::string key);
    std::vector<CapturedPackets>& getFindedPackets();
    void displayPacket(int id);
private:
    std::shared_ptr<SearchModel> model;
    std::shared_ptr<SearchView> view;
    std::shared_ptr<MainController> mainController;
    std::vector<CapturedPackets> findedPackets;
    std::string byteToHex(unsigned char byte);
};


#endif //SEARCHCONTROLLER_H
