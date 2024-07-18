#ifndef ANOTHERVIEW_H
#define ANOTHERVIEW_H

#include "View.h"

class AnotherView : public View {
public:
    virtual ~AnotherView() {}
    void draw() override;
};

#endif // ANOTHERVIEW_H
