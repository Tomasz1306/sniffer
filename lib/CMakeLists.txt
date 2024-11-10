# CMakeLists.txt w katalogu lib

# Dodaj bibliotekÄ™ imgui
add_library(imgui
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imconfig.h
    imgui/imgui.h
    imgui/imgui_internal.h
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    implot/implot.h
    implot/implot_internal.h
    implot/implot.cpp
    implot/implot_demo.cpp
    implot/implot_items.cpp
)

# Dodaj bibliotekÄ™ imgui_backend
add_library(imgui_backend
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    imgui/backends/imgui_impl_glfw.h
    imgui/backends/imgui_impl_opengl3.h
    imgui/backends/imgui_impl_opengl3_loader.h
)

# Dodaj zaleĹĽnoĹ›ci
target_link_libraries(imgui_backend glfw OpenGL::GL)
target_link_libraries(imgui imgui_backend)

# Dodaj Ĺ›cieĹĽki do plikĂłw nagĹ‚Ăłwkowych
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui ${CMAKE_CURRENT_SOURCE_DIR}/implot ${CMAKE_CURRENT_SOURCE_DIR}/imgui/backends)
target_include_directories(imgui_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}imgui/backends ${CMAKE_CURRENT_SOURCE_DIR}/imgui)


