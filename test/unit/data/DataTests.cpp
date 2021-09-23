// Copyright (c) 2021 Eduardo Ponz Segrelles.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <gtest/gtest.h>

#include <gnss_interface/data.hpp>
#include <gnss_interface/types.hpp>

using namespace eduponz::gnss_interface;

TEST(DataTests, NMEA0183DataComparisonOperators)
{
    NMEA0183Data data_1;
    NMEA0183Data data_2;

    ASSERT_EQ(data_1, data_2);

    data_1.kind = NMEA0183DataKind::GPGGA;
    ASSERT_NE(data_1, data_2);

    data_1.message = "message";
    ASSERT_NE(data_1, data_2);
}

TEST(DataTests, GPGGADataComparisonOperators)
{
    GPGGAData data_1;
    GPGGAData data_2;

    ASSERT_EQ(data_1, data_2);

    data_1.altitude = 123;
    ASSERT_NE(data_1, data_2);

    data_1.horizontal_precision = 123;
    ASSERT_NE(data_1, data_2);

    data_1.satellites_on_view = 123;
    ASSERT_NE(data_1, data_2);

    data_1.fix = 2;
    ASSERT_NE(data_1, data_2);

    data_1.longitude = 123.0;
    ASSERT_NE(data_1, data_2);

    data_1.latitude = 123.0;
    ASSERT_NE(data_1, data_2);

    data_1.timestamp = 123.0;
    ASSERT_NE(data_1, data_2);

    data_1.message = "message";
    ASSERT_NE(data_1, data_2);
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
