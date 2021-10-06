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

find_package(PythonInterp 3 REQUIRED)

add_executable(system_tests system_tests.cpp)
target_link_libraries(system_tests opennmea)

set(SYSTEM_TESTS_RUN_DIR ${CMAKE_CURRENT_BINARY_DIR})

configure_file(system_tests.py ${SYSTEM_TESTS_RUN_DIR}/system_tests.py COPYONLY)
configure_file(send2serial.py ${SYSTEM_TESTS_RUN_DIR}/send2serial.py COPYONLY)
configure_file(system_tests.yaml.in ${SYSTEM_TESTS_RUN_DIR}/system_tests.yaml @ONLY)
configure_file(system_tests.schema.json ${SYSTEM_TESTS_RUN_DIR}/system_tests.schema.json COPYONLY)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/nmea_sentences DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/validation DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/results)

set(SYSTEM_TEST_LIST
    gpgga_read_some_and_close
    port_closed_externally
    stop_sending_data
    late_sending)

foreach(test_name ${SYSTEM_TEST_LIST})

    add_test(NAME SystemTests.${test_name}
            COMMAND ${PYTHON_EXECUTABLE} ${SYSTEM_TESTS_RUN_DIR}/system_tests.py
            -c ${SYSTEM_TESTS_RUN_DIR}/system_tests.yaml
            -t ${test_name}
            -d)

endforeach()
