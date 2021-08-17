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

cmake_minimum_required(VERSION 2.8.12)

# Supress developer warnings
if(NOT CMAKE_VERSION VERSION_LESS 3.0)
    cmake_policy(SET CMP0048 NEW)
endif()

set(LIB_NAME "gnss_l86_interface")


set(EXAMPLE_NAME "${LIB_NAME}_example")
project(${EXAMPLE_NAME})

message(STATUS "Configuring GNSS L86 Interface Example...")

# If the examples is build alongside the library, then the library exists; else, we need to find it.
if (BUILD_EXAMPLES)
    set(${LIB_NAME}_FOUND TRUE)
elseif (NOT ${LIB_NAME}_FOUND)
    find_package(${LIB_NAME})
endif()

# Get example sources
set(EXAMPLE_SOURCES_CPP
    gnss_l86_example.cpp)

# Create executable
add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCES_CPP})

# Link executable with library
target_link_libraries(${EXAMPLE_NAME} ${LIB_NAME})

# Install example executable
install(TARGETS ${EXAMPLE_NAME}
    RUNTIME DESTINATION examples/${BIN_INSTALL_DIR})
