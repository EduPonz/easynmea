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

add_executable(serial_interface_tests SerialInterfaceTests.cpp)

target_include_directories(serial_interface_tests PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/src/cpp)

target_link_libraries(serial_interface_tests PUBLIC
    GTest::GTest
    GTest::Main
    ${GMOCK_BOTH_LIBRARIES})

set(SERIAL_INTERFACE_TEST_LIST
    # open() tests
    openSuccess
    openOpened
    openWrongPort
    openWrongBaudrate
    # is_open() tests
    is_openOpened
    is_openClosed
    # close() tests
    closeSuccess
    closeClosed
    closeAsioError
    # read_line tests
    read_lineSuccess
    read_lineClosed
    read_lineReadError)

foreach(test_name ${SERIAL_INTERFACE_TEST_LIST})

    add_test(NAME SerialInterfaceTests.${test_name}
            COMMAND serial_interface_tests
            --gtest_filter=SerialInterfaceTests.${test_name}:*/SerialInterfaceTests.${test_name}/*)

endforeach()
