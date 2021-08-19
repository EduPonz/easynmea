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

set(ASIO_REQUIRED_VERSION 1.10.8)

find_package(Asio CONFIG QUIET)

# If Asio was not found, search for the header file asio.hpp in the common CMake directories.
if(NOT Asio_FOUND)
    find_path(Asio_INCLUDE_DIR NAMES asio.hpp)
else()
    set(Asio_FOUND_PACKAGE ON)
endif()

# Asio local version not found
if(Asio_INCLUDE_DIR)
    file(READ "${Asio_INCLUDE_DIR}/asio/version.hpp" VERSION_INCLUDE)
    string(REGEX MATCH "#define ASIO_VERSION ([0-9]+)" REGEX_VERSION ${VERSION_INCLUDE})
    set(ASIO_VERSION ${CMAKE_MATCH_1})
    math(EXPR ASIO_PATCH_VERSION ${ASIO_VERSION}%100)
    math(EXPR ASIO_MINOR_VERSION ${ASIO_VERSION}/100%1000)
    math(EXPR ASIO_MAYOR_VERSION ${ASIO_VERSION}/100000)
    set(ASIO_VERSION "${ASIO_MAYOR_VERSION}.${ASIO_MINOR_VERSION}.${ASIO_PATCH_VERSION}")

    if(${ASIO_VERSION} VERSION_LESS ${ASIO_REQUIRED_VERSION})
        unset(Asio_INCLUDE_DIR)
        unset(Asio_FOUND)
        set(Asio_FOUND_PACKAGE OFF)
        message(FATAL_ERROR
            "Found Asio version ${ASIO_VERSION}, which is not compatible with requested one. "
            "Minimum required Asio version: ${ASIO_REQUIRED_VERSION}"
        )
    endif()
endif()

if(Asio_INCLUDE_DIR AND (NOT Asio_FOUND_PACKAGE))
    include(FindPackageHandleStandardArgs)
    # Asio is considered to be found if Asio_INCLUDE_DIR is valid.
    find_package_handle_standard_args(Asio DEFAULT_MSG Asio_INCLUDE_DIR)
    mark_as_advanced(Asio_INCLUDE_DIR)
    message(STATUS "Found Asio ${ASIO_VERSION}: ${Asio_INCLUDE_DIR}")
else()
    message(STATUS "Cannot find package Asio")
endif()
