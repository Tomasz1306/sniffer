#ifndef MAINVIEW_H
#define MAINVIEW_H

#include "View.h"

class MainView : public View {
public:
    virtual ~MainView() {}
    void draw() override;
};

#endif // MAINVIEW_H
