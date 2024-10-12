//
// Created by tomasz on 02.10.24.
//

#ifndef DATABASEVIEW_H
#define DATABASEVIEW_H

#include "controllers/DataBaseController.h"
#include "views/View.h"

#include <memory>

class DataBaseController;

class DataBaseView : public View{
public:
    DataBaseView();

    void draw(std::shared_ptr<DataBaseController> controller);
    void drawProgressBar(std::shared_ptr<DataBaseController> controller);
private:

    char host[128] = "localhost";
    char port[128] = "3306";
    char username[128] = "root";
    char password[128] = "root";
    char newDatabase[128] = "";
    bool newDatabaseWindow{false};

};

#endif //DATABASEVIEW_H
