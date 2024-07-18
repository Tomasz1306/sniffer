#include "services/RenderService.h"
#include "GLFW/glfw3.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

RenderService::RenderService(std::vector<std::shared_ptr<View>>& views) : views(views) {}

void RenderService::render() {
    for (auto view : views) {
        view->draw();
    }
}
