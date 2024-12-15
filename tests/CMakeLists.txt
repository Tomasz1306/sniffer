#add_executable(LogTest LogTest.cpp ../src/models/LogModel.cpp ../src/controllers/LogController.cpp)
#
#target_link_libraries(LogTest
#        gtest
#        gtest_main
#        pthread
#)
#
#add_test(NAME LogTest COMMAND LogTest)
#tests/CMakeLists.txt
file(GLOB_RECURSE SRC_FILES ../src/*.cpp)
add_executable(Tests LogTest.cpp
        DeviceTest.cpp
        AnalyzerTest.cpp
        ${SRC_FILES}
)

target_link_libraries(Tests
        PcapPlusPlus::Pcap++
        imgui
        glfw
        OpenGL::GL
        mysqlcppconn
        pthread
        gtest
        gtest_main
)
#add_test(NAME log_test COMMAND LogTest)
add_test(NAME tests COMMAND Tests)