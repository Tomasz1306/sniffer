# Install script for directory: /home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "XDevAPIDev" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/mysqlx/devapi" TYPE FILE FILES
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/common.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/error.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/row.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/result.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/executable.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/document.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/settings.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/crud.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/collection_crud.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/table_crud.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/collations.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/mysql_charsets.h"
    "/home/tomasz/Desktop/projects/sniffer/lib/mysql-connector-cpp/include/mysqlx/devapi/mysql_collations.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/tomasz/Desktop/projects/sniffer/cmake-build-debug/lib/mysql-connector-cpp/include/mysqlx/devapi/detail/cmake_install.cmake")

endif()

