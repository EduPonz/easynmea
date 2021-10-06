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

add_executable(opennmea_impl_tests OpenNmeaImplTests.cpp)

target_include_directories(opennmea_impl_tests PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/cpp)

target_link_libraries(opennmea_impl_tests PUBLIC
    GTest::GTest
    GTest::Main
    ${GMOCK_BOTH_LIBRARIES}
    opennmea)

set(OPENNMEA_IMPL_TEST_LIST
    # open() tests
    openSuccess
    openOpened
    openWrongPort
    # is_open() tests
    is_openOpened
    is_openClosed
    # close() tests
    closeSuccess
    closeClosed
    # wait_for_data() tests
    wait_for_dataData
    wait_for_dataClosed
    wait_for_dataDataEmptyMask
    wait_for_dataError
    # take_next() tests
    take_next
    # ~OpenNmeaImpl() tests
    destroyNoClose)

foreach(test_name ${OPENNMEA_IMPL_TEST_LIST})

    add_test(NAME OpenNmeaImplTests.${test_name}
            COMMAND opennmea_impl_tests
            --gtest_filter=OpenNmeaImplTests.${test_name}:*/OpenNmeaImplTests.${test_name}/*)

endforeach()
