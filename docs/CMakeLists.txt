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

####################################################################################################
# Build Doxygen documentation
####################################################################################################

file(GLOB_RECURSE HPP_FILES "${PROJECT_SOURCE_DIR}/include/**/*.h*")
set(INDEX_FILE ${PROJECT_SOURCE_DIR}/README.md)
set(LICENSE_FILE ${PROJECT_SOURCE_DIR}/LICENSE.md)
set(PROJECT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(PROJECT_SOURCE_DOCS_DIR ${PROJECT_SOURCE_DIR}/docs)
set(PROJECT_BINARY_DOCS_DIR ${PROJECT_BINARY_DIR}/docs)

find_package(Doxygen REQUIRED)
# Doxygen related variables
set(DOXYGEN_INPUT_DIR "${PROJECT_INCLUDE_DIR}")
set(DOXYGEN_OUTPUT_DIR "${PROJECT_BINARY_DOCS_DIR}/doxygen")
set(DOXYGEN_OUTPUT_XML_DIR "${DOXYGEN_OUTPUT_DIR}/xml")
set(DOXYGEN_OUTPUT_HTML_DIR "${DOXYGEN_OUTPUT_DIR}/html")
set(DOXYGEN_INDEX_FILE "${DOXYGEN_OUTPUT_XML_DIR}/index.xml")
set(DOXYFILE_IN "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in")
set(DOXYFILE_OUT ${DOXYGEN_OUTPUT_DIR}/Doxyfile)

# Create doxygen directories
add_custom_target(doxygen-doc-dirs
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DOCS_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_XML_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DOXYGEN_OUTPUT_HTML_DIR}
    COMMENT "Creating doxygen documentation directories" VERBATIM)

# Configure doxygen
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

# Doxygen command
add_custom_command(OUTPUT ${DOXYGEN_INDEX_FILE}
    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
    DEPENDS ${HPP_FILES}
    MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
    COMMENT "Generating doxygen documentation")

# Generate API reference
add_custom_target(doxygen ALL
    DEPENDS ${DOXYGEN_INDEX_FILE}
    COMMENT "Generated API documentation with doxygen" VERBATIM)
add_dependencies(doxygen doxygen-doc-dirs)

# Install doxygen generated XML files
install(DIRECTORY ${DOXYGEN_OUTPUT_XML_DIR}
    DESTINATION docs/${PROJECT_NAME}/doxygen
    COMPONENT gnss_interface-doxygen)

install(DIRECTORY ${DOXYGEN_OUTPUT_HTML_DIR}
    DESTINATION docs/${PROJECT_NAME}/doxygen
    COMPONENT gnss_interface-doxygen)

####################################################################################################
# Build Sphinx documentation
####################################################################################################

# Compile code snippets
add_executable(sphinx_docs_snippets rst/snippets/snippets.cpp)
target_link_libraries(sphinx_docs_snippets gnss_interface)

# Find sphinx
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules" ${CMAKE_MODULE_PATH})
find_package(Sphinx REQUIRED)

set(DOCS_BUILDER html)

# Generate the sphinx documentation
set(SPHINX_OUTPUT_HTML_DIR ${PROJECT_BINARY_DOCS_DIR}/sphinx/html)

add_custom_target(sphinx-doc-dirs
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SPHINX_OUTPUT_HTML_DIR}
    COMMENT "Creating sphinx documentation directories" VERBATIM)

add_custom_target(Sphinx ALL
    COMMAND
    ${SPHINX_EXECUTABLE} -b ${DOCS_BUILDER}
    # Tell Breathe where to find the Doxygen output
    -D breathe_projects.gnss_interface=${DOXYGEN_OUTPUT_XML_DIR}
    $<$<STREQUAL:"${CMAKE_BUILD_TYPE}","Debug">:-Dtodo_include_todos=1>
    -d "${PROJECT_BINARY_DOCS_DIR}/doctrees"
    ${PROJECT_SOURCE_DOCS_DIR}
    ${SPHINX_OUTPUT_HTML_DIR}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS doxygen
    COMMENT "Generating documentation with Sphinx")
add_dependencies(Sphinx sphinx-doc-dirs)

# Install the generated docs
install(DIRECTORY ${SPHINX_OUTPUT_HTML_DIR}
    DESTINATION docs/${PROJECT_NAME}/sphinx
    COMPONENT gnss_interface-sphinx
    PATTERN ".buildinfo" EXCLUDE)
