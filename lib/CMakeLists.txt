# CMakeLists.txt w katalogu lib

# Dodaj bibliotekę imgui
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
)

# Dodaj bibliotekę imgui_backend
add_library(imgui_backend
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_glfw.h
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_opengl3_loader.h
)



# Dodaj zależności
target_link_libraries(imgui_backend glfw OpenGL::GL)
target_link_libraries(imgui imgui_backend)

# Dodaj ścieżki do plików nagłówkowych
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_include_directories(imgui_backend PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/backends ${CMAKE_CURRENT_SOURCE_DIR}/imgui)


