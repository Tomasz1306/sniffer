cmake_minimum_required(VERSION 3.12)
project(Sniffer)

#DEPENDENCIES: sudo apt-get install libglfw3-dev, sudo apt-get install libgl1-mesa-dev

#set(CMAKE_PREFIX_PATH "lib/PcapPlusPlus/" ${CMAKE_PREFIX_PATH})
# Włącz kompilację C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_subdirectory(lib/PcapPlusPlus)
add_library(PcapPlusPlus::Pcap++ ALIAS Pcap++)
# Znajdź wymagane pakiety
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
#find_package(PcapPlusPlus REQUIRED)

# Dodaj ścieżki do plików nagłówkowych
include_directories(include)
include_directories(lib/imgui)

#include_directories(/home/linuxbrew/.linuxbrew/Cellar/pcapplusplus/23.09/include)

# Dodaj katalogi z plikami źródłowymi
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

# Dodaj katalogi z bibliotekami
add_subdirectory(lib)

# Stwórz plik wykonywalny
add_executable(sniffer ${SOURCES} ${HEADERS})


# Połącz biblioteki z plikiem wykonywalnym
target_link_libraries(sniffer PUBLIC PcapPlusPlus::Pcap++ imgui glfw OpenGL::GL )
