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

# Compile code snippets. This makes sure that all snippets at least compile and don't get outdated.
add_executable(
    ${PROJECT_NAME}-documentation-test
    ../rst/snippets/snippets.cpp
)
target_link_libraries(
    ${PROJECT_NAME}-documentation-test
    ${PROJECT_NAME}
)

####################################################################################################
# Add tests
####################################################################################################

find_package(Python COMPONENTS Interpreter Development)
if(NOT(${Python_FOUND} AND (${Python_VERSION} VERSION_GREATER "3.6")))
    message(FATAL_ERROR "Building the documentation tests requires Python 3.7")
endif()

# Check docs style
find_program(DOC8 doc8 DOC "path to doc8 executable")
message(STATUS "doc8 found: ${DOC8}")
if(DOC8)
    add_test(
        NAME documentation.line_length
        COMMAND ${DOC8}
        --max-line-length 120 ${PROJECT_SOURCE_DOCS_DIR})
else()
    message(FATAL_ERROR "Could not find doc8")
endif()

# Check docs spelling
add_test(NAME documentation.spell_check
    COMMAND
    ${SPHINX_EXECUTABLE} -Q -W --keep-going
    -D breathe_projects.gnss_interface=${DOXYGEN_OUTPUT_DIR}/xml
    -b spelling
    -d "${PROJECT_BINARY_DOCS_DIR}/doctrees"
    ${PROJECT_SOURCE_DOCS_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/spelling)
