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

add_executable(easynmea_coder_tests EasyNmeaCoderTests.cpp)

target_include_directories(easynmea_coder_tests PRIVATE
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/cpp)

target_link_libraries(easynmea_coder_tests PUBLIC
    GTest::GTest
    GTest::Main
    ${GMOCK_BOTH_LIBRARIES}
    easynmea)

set(EASYNMEA_IMPL_TEST_LIST
    # decoder() tests
    decodeGPGGAValidNE
    decodeGPGGAValidNW
    decodeGPGGAValidSE
    decodeGPGGAValidSW
    decodeGPGGAValidNoAgeOfDiffGPS
    decodeGPGGAValidEmptyAgeOfDiffGPSNoDiffRefStation
    decodeGPGGAValidNoDiffRefStation
    decodeGPGGAValidNoOptionals
    decodeInvalidSentenceID
    decodeGPGGAInvalidTime
    decodeGPGGAInvalidLatitudeLength
    decodeGPGGAInvalidLatitudeDegrees
    decodeGPGGAInvalidLatitudeMinutes
    decodeGPGGAInvalidLongitudeLength
    decodeGPGGAInvalidLongitudeDegrees
    decodeGPGGAInvalidLongitudeMinutes
    decodeGPGGAInvalidAltitudeUnits
    decodeGPGGAInvalidHeightUnits
    decodeGPGGAInvalidChecksum
    decodeGPGGANoTime
    decodeGPGGANoLatitude
    decodeGPGGANoLongitude
    decodeGPGGANoFix
    decodeGPGGANoNumberOfSatellites
    decodeGPGGANoHDOP
    decodeGPGGANoAltitude
    decodeGPGGANoHeight
    decodeGPGGANoChecksum
    decodeUnsupportedSentence
    decodeEmptySentence
    decodeOnlyChecksumSentence
    decodeOnlyAstheriscSentence)

foreach(test_name ${EASYNMEA_IMPL_TEST_LIST})

    add_test(NAME EasyNmeaCoderTests.${test_name}
            COMMAND easynmea_coder_tests
            --gtest_filter=EasyNmeaCoderTests.${test_name}:*/EasyNmeaCoderTests.${test_name}/*)

endforeach()
