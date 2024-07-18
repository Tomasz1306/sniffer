cmake_minimum_required(VERSION 3.10)
project(MyProject)

# Włącz kompilację C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Znajdź wymagane pakiety
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)

# Dodaj ścieżkę do plików nagłówkowych
include_directories(include)
include_directories(lib/imgui)

# Dodaj katalogi z plikami źródłowymi
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# Dodaj katalogi z bibliotekami
add_subdirectory(lib)

# Stwórz plik wykonywalny
add_executable(sniffer ${SOURCES} ${HEADERS})

# Połącz biblioteki z plikiem wykonywalnym
target_link_libraries(sniffer imgui glfw OpenGL::GL)
