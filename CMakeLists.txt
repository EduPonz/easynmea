# Copyright (c) 2021 Eduardo Ponz Segrelles.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

###############################################################################
# CMake build rules for OpenNMEA Lib
###############################################################################
cmake_minimum_required(VERSION 3.10)
cmake_policy(VERSION 3.10...3.20)

###############################################################################
# Project
###############################################################################
set(PROJECT_NAME_LARGE "OpenNMEA")

project(opennmea VERSION 0.1.0
    DESCRIPTION "${PROJECT_NAME_LARGE} library provides a simple and easy-to-use API for interfacing with NMEA 0183 UART modules")

set(${PROJECT_NAME}_DESCRIPTION_SUMMARY "C++ library for interfacing with NMEA 1083 modules")

message(STATUS "Configuring ${PROJECT_NAME_LARGE}")
message(STATUS "Version: ${PROJECT_VERSION}")

###############################################################################
# Load CMake modules
###############################################################################
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/modules)

###############################################################################
# CMake flags
###############################################################################
if(CMAKE_COMPILER_IS_GNUCXX)
    option(GCC_CODE_COVERAGE "Sets gcc code coverage flags" OFF)
    if (GCC_CODE_COVERAGE)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
    endif()
endif()

###############################################################################
# Load external projects.
###############################################################################
find_package(Threads REQUIRED)
find_package(Asio REQUIRED)

###############################################################################
# Installation paths
###############################################################################
set(BIN_INSTALL_DIR bin/ CACHE PATH "Installation directory for binaries")
set(INCLUDE_INSTALL_DIR include/ CACHE PATH "Installation directory for C++ headers")
set(LIB_INSTALL_DIR lib${LIB_SUFFIX}/ CACHE PATH "Installation directory for libraries")
set(DATA_INSTALL_DIR share/ CACHE PATH "Installation directory for data")
if(WIN32)
    set(LICENSE_INSTALL_DIR . CACHE PATH "Installation directory for licenses")
else()
    set(LICENSE_INSTALL_DIR ${DATA_INSTALL_DIR}/${PROJECT_NAME} CACHE PATH "Installation directory for licenses")
endif()

###############################################################################
# Default shared libraries
###############################################################################
# Global flag to cause add_library() to create shared libraries if on.
# If set to true, this will cause all libraries to be built shared
# unless the library was explicitly added as a static library.
option(BUILD_SHARED_LIBS "Create shared libraries by default" ON)

###############################################################################
# Compile library
###############################################################################
add_subdirectory(src/cpp)

###############################################################################
# Test
###############################################################################
option(BUILD_TESTS "Build OpenNMEA library and documentation tests" OFF)
option(BUILD_LIBRARY_TESTS "Build OpenNMEA library tests" OFF)
option(BUILD_DOCUMENTATION_TESTS "Build OpenNMEA documentation tests" OFF)

if (BUILD_TESTS)
    set(BUILD_LIBRARY_TESTS ON)
    set(BUILD_DOCUMENTATION_TESTS ON)
endif()

if(BUILD_LIBRARY_TESTS OR BUILD_DOCUMENTATION_TESTS)
    # CTest needs to be included here, otherwise it is not possible to run the tests from the root
    # of the build directory
    enable_testing()
    include(CTest)
    find_package(GTest REQUIRED)
    find_package(GMock REQUIRED)
endif()

if (BUILD_LIBRARY_TESTS)
    add_subdirectory(test)
endif()

###############################################################################
# Build documentation
###############################################################################
option(BUILD_DOCUMENTATION "Build ${PROJECT_NAME} documentation" OFF)
if(BUILD_DOCUMENTATION OR BUILD_DOCUMENTATION_TESTS)
    set(BUILD_DOCUMENTATION ON)
    add_subdirectory(docs)
endif()

###############################################################################
# Build examples
###############################################################################
option(BUILD_EXAMPLES "Build ${PROJECT_NAME} examples" OFF)
if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

###############################################################################
# Packaging
###############################################################################
# Install license
install(FILES ${PROJECT_SOURCE_DIR}/LICENSE.md
    DESTINATION ${LICENSE_INSTALL_DIR}
    COMPONENT licenses)
