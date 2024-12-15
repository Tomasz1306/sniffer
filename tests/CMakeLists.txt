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
add_executable(LogTest LogTest.cpp ../src/models/LogModel.cpp ../src/views/LogView.cpp ../src/controllers/LogController.cpp)  # Add your test source files
target_link_libraries(LogTest PcapPlusPlus::Pcap++
        imgui
        glfw
        OpenGL::GL
        mysqlcppconn
        pthread
        gtest
        gtest_main) # Link with Google Test
add_test(NAME log_test COMMAND LogTest) # Add the test to CMake