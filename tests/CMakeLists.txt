file(GLOB_RECURSE SRC_FILES ../src/*.cpp)
add_executable(Tests LogTest.cpp
        DeviceTest.cpp
        AnalyzerTest.cpp
        FilterTest.cpp
        SearchTest.cpp
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