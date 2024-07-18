#ifndef RENDERSERVICE_H
#define RENDERSERVICE_H

#include <vector>
#include <memory>

#include "views/View.h"

class RenderService {
public:
    RenderService(std::vector<std::shared_ptr<View>>& views);
    void render();
private:
    std::vector<std::shared_ptr<View>>& views;
};

#endif // RENDERSERVICE_H
