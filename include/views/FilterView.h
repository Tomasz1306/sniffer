//
// Created by tomasz on 8/10/24.
//

#ifndef FILTERVIEW_H
#define FILTERVIEW_H
#include "views/View.h"
#include "models/FilterModel.h"

#include <vector>
#include <string>
#include <memory>

class FilterView : public View {
public:
    FilterView();
    virtual ~FilterView() {}
    void draw(std::shared_ptr<FilterModel> model);

    void addressIpSection(std::shared_ptr<FilterModel> model);
    void addressMacSection(std::shared_ptr<FilterModel> model);
    void tcpFlagsSection(std::shared_ptr<FilterModel> model);
    void protocolsSection(std::shared_ptr<FilterModel> model);
    void displayOption(std::shared_ptr<FilterModel> model);

private:
    std::vector<std::string> items;
    char buf_ip[64]{""};
    char buf_mac[64]{""};
};


#endif //FILTERVIEW_H
