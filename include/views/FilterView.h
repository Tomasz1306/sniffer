//
// Created by tomasz on 8/10/24.
//

#ifndef FILTERVIEW_H
#define FILTERVIEW_H
#include "views/View.h"

#include <vector>
#include <string>

class FilterView :public View {
public:
    FilterView() {}
    virtual ~FilterView() {}
    void draw();
private:
    std::vector<std::string> items;
    char buf[64]{""};
};


#endif //FILTERVIEW_H
