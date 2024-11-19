//
// Created by tomasz on 11/12/24.
//

#ifndef SEARCHVIEW_H
#define SEARCHVIEW_H

#include "controllers/SearchController.h"
#include "View.h"

#include <memory>

class SearchController;

class SearchView : public View {
public:
    SearchView();
    void draw(std::shared_ptr<SearchController> controller);

private:
    char searchText[100];
};

#endif //SEARCHVIEW_H
