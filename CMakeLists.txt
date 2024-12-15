#cmake_minimum_required(VERSION 3.12)
#project(Sniffer)
#
##DEPENDENCIES: sudo apt-get install libglfw3-dev libssl-dev libgl1-mesa-dev libpcap-dev libmysqlcppconn-dev
##https://github.com/seladb/PcapPlusPlus.git
##https://github.com/ocornut/imgui.git
##https://github.com/epezent/implot.git
##https://github.com/mysql/mysql-connector-cpp.git
##set(CMAKE_PREFIX_PATH "lib/PcapPlusPlus/" ${CMAKE_PREFIX_PATH})
#
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED True)
#add_subdirectory(lib/PcapPlusPlus)
#add_library(PcapPlusPlus::Pcap++ ALIAS Pcap++)
#
#find_package(OpenGL REQUIRED)
#find_package(glfw3 3.3 REQUIRED)
#find_package(GTest REQUIRED)
##find_package(PcapPlusPlus REQUIRED)
#
#include_directories(include)
#include_directories(lib/imgui)
#
#
#file(GLOB_RECURSE SOURCES "src/*.cpp")
#file(GLOB_RECURSE HEADERS "include/*.h")
#
#add_subdirectory(lib)
##
#add_executable(sniffer ${SOURCES} ${HEADERS})
#
## Określenie `ENABLE_EXPORTS` dla wykonywalnego targetu
#set_target_properties(sniffer PROPERTIES ENABLE_EXPORTS ON)
#target_link_libraries(sniffer PUBLIC PcapPlusPlus::Pcap++ imgui glfw OpenGL::GL mysqlcppconn GTest::GTest GTest::Main pthread)
#
#enable_testing()
#add_subdirectory(tests)



cmake_minimum_required(VERSION 3.12)
project(Sniffer)

#DEPENDENCIES: sudo apt-get install libglfw3-dev libssl-dev libgl1-mesa-dev libpcap-dev libmysqlcppconn-dev
#https://github.com/seladb/PcapPlusPlus.git
#https://github.com/ocornut/imgui.git
#https://github.com/epezent/implot.git
#https://github.com/mysql/mysql-connector-cpp.git
#set(CMAKE_PREFIX_PATH "lib/PcapPlusPlus/" ${CMAKE_PREFIX_PATH})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_subdirectory(lib/PcapPlusPlus)
add_library(PcapPlusPlus::Pcap++ ALIAS Pcap++)

find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GTest REQUIRED)
#find_package(PcapPlusPlus REQUIRED)

include_directories(include)
include_directories(lib/imgui)


file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h")

add_subdirectory(lib)
#
add_executable(sniffer ${SOURCES} ${HEADERS})
# Link with the necessary libraries for your main executable
target_link_libraries(sniffer PUBLIC
        PcapPlusPlus::Pcap++
        imgui
        glfw
        OpenGL::GL
        mysqlcppconn
        pthread
        gtest
        gtest_main
)
enable_testing()
add_subdirectory(tests)