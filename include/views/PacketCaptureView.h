#ifndef PACKETCAPTUREVIEW_H
#define PACKETCAPTUREVIEW_H

#include "imgui.h"
#include "View.h"

class PacketCaptureView : public View {


public:

    virtual ~PacketCaptureView() {};
    void draw() override;
};


#endif // PACKETSNIFFWINDOW