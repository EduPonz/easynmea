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

#include <memory>
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <gnss_interface/types.hpp>
#include <gnss_interface/GnssInterface.hpp>

#include "GnssInterfaceImplMock.hpp"

using namespace eduponz::gnss_interface;

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class GnssInterfaceTest : public GnssInterface
{
public:

    GnssInterfaceTest()
        : GnssInterface()
    {}

    void set_impl(std::unique_ptr<GnssInterfaceImplMock> impl)
    {
        impl_ = std::move(impl);
    }
};

TEST(GnssInterfaceTests, openOk)
{
    const char* port = "port";
    long baudrate = 9600;

    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();

    EXPECT_CALL(*impl, open(port, baudrate))
        .WillOnce(Return(ReturnCode::RETURN_CODE_OK));

    gnss_interface.set_impl(std::move(impl));

    EXPECT_EQ(gnss_interface.open(port, baudrate), ReturnCode::RETURN_CODE_OK);
}

TEST(GnssInterfaceTests, openError)
{
    const char* port = "port";
    long baudrate = 9600;

    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();

    EXPECT_CALL(*impl, open(port, baudrate))
        .WillOnce(Return(ReturnCode::RETURN_CODE_ERROR));

    gnss_interface.set_impl(std::move(impl));

    EXPECT_EQ(gnss_interface.open(port, baudrate), ReturnCode::RETURN_CODE_ERROR);
}

TEST(GnssInterfaceTests, openIllegal)
{
    const char* port = "port";
    long baudrate = 9600;

    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();

    EXPECT_CALL(*impl, open(port, baudrate))
        .WillOnce(Return(ReturnCode::RETURN_CODE_ILLEGAL_OPERATION));

    gnss_interface.set_impl(std::move(impl));

    EXPECT_EQ(gnss_interface.open(port, baudrate), ReturnCode::RETURN_CODE_ILLEGAL_OPERATION);
}

TEST(GnssInterfaceTests, is_openOpened)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();

    EXPECT_CALL(*impl, is_open)
        .WillOnce(Return(true));

    gnss_interface.set_impl(std::move(impl));

    EXPECT_EQ(gnss_interface.is_open(), true);
}

TEST(GnssInterfaceTests, is_openClosed)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();

    EXPECT_CALL(*impl, is_open)
        .WillOnce(Return(false));

    gnss_interface.set_impl(std::move(impl));

    EXPECT_EQ(gnss_interface.is_open(), false);
}

TEST(GnssInterfaceTests, closeOk)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();

    EXPECT_CALL(*impl, close)
        .WillOnce(Return(ReturnCode::RETURN_CODE_OK));

    gnss_interface.set_impl(std::move(impl));

    ASSERT_EQ(gnss_interface.close(), ReturnCode::RETURN_CODE_OK);
}

TEST(GnssInterfaceTests, closeError)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();

    EXPECT_CALL(*impl, close)
        .WillOnce(Return(ReturnCode::RETURN_CODE_ERROR));

    gnss_interface.set_impl(std::move(impl));

    ASSERT_EQ(gnss_interface.close(), ReturnCode::RETURN_CODE_ERROR);
}

TEST(GnssInterfaceTests, closeIllegal)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();

    EXPECT_CALL(*impl, close)
        .WillOnce(Return(ReturnCode::RETURN_CODE_ILLEGAL_OPERATION));

    gnss_interface.set_impl(std::move(impl));

    ASSERT_EQ(gnss_interface.close(), ReturnCode::RETURN_CODE_ILLEGAL_OPERATION);
}

TEST(GnssInterfaceTests, take_nextOk)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();
    GPGGAData gpgga;
    GPGGAData gpgga_ret;
    gpgga_ret.message = "hello";
    gpgga_ret.timestamp = 123;
    gpgga_ret.latitude = 123;
    gpgga_ret.longitude = 123;
    gpgga_ret.fix = 123;
    gpgga_ret.satellites_on_view = 123;
    gpgga_ret.horizontal_precision = 123;
    gpgga_ret.altitude = 123;

    ASSERT_NE(gpgga.altitude, gpgga_ret.altitude);

    EXPECT_CALL(*impl, take_next(gpgga))
        .WillOnce(DoAll(SetArgReferee<0>(gpgga_ret), Return(ReturnCode::RETURN_CODE_OK)));

    gnss_interface.set_impl(std::move(impl));

    ASSERT_EQ(gnss_interface.take_next(gpgga), ReturnCode::RETURN_CODE_OK);

    ASSERT_EQ(gpgga, gpgga_ret);
}

TEST(GnssInterfaceTests, take_nextNoData)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();
    GPGGAData gpgga;

    EXPECT_CALL(*impl, take_next(gpgga))
        .WillOnce(Return(ReturnCode::RETURN_CODE_NO_DATA));

    gnss_interface.set_impl(std::move(impl));

    ASSERT_EQ(gnss_interface.take_next(gpgga), ReturnCode::RETURN_CODE_NO_DATA);

    ASSERT_EQ(gpgga.message, "");
    ASSERT_EQ(gpgga.kind, NMEA0183DataKind::GPGGA);
    ASSERT_EQ(gpgga.timestamp, 0);
    ASSERT_EQ(gpgga.latitude, 0);
    ASSERT_EQ(gpgga.longitude, 0);
    ASSERT_EQ(gpgga.fix, 0);
    ASSERT_EQ(gpgga.satellites_on_view, 0);
    ASSERT_EQ(gpgga.horizontal_precision, 0);
    ASSERT_EQ(gpgga.altitude, 0);
}

TEST(GnssInterfaceTests, wait_for_dataOk)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();
    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();
    std::chrono::milliseconds timeout(123);

    EXPECT_CALL(*impl, wait_for_data(_, timeout))
        .WillOnce(Return(ReturnCode::RETURN_CODE_OK));

    gnss_interface.set_impl(std::move(impl));

    ASSERT_EQ(gnss_interface.wait_for_data(mask, timeout), ReturnCode::RETURN_CODE_OK);
}

TEST(GnssInterfaceTests, wait_for_dataTimeout)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();
    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();
    std::chrono::milliseconds timeout(123);

    EXPECT_CALL(*impl, wait_for_data(_, timeout))
        .WillOnce(Return(ReturnCode::RETURN_CODE_TIMEOUT));

    gnss_interface.set_impl(std::move(impl));

    ASSERT_EQ(gnss_interface.wait_for_data(mask, timeout), ReturnCode::RETURN_CODE_TIMEOUT);
}

TEST(GnssInterfaceTests, wait_for_dataTimeoutDefault)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();
    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();

    EXPECT_CALL(*impl, wait_for_data)
        .WillOnce(Return(ReturnCode::RETURN_CODE_TIMEOUT));

    gnss_interface.set_impl(std::move(impl));

    ASSERT_EQ(gnss_interface.wait_for_data(mask), ReturnCode::RETURN_CODE_TIMEOUT);
}

TEST(GnssInterfaceTests, wait_for_dataIllegal)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();
    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();
    std::chrono::milliseconds timeout(123);

    EXPECT_CALL(*impl, wait_for_data(_, timeout))
        .WillOnce(Return(ReturnCode::RETURN_CODE_ILLEGAL_OPERATION));

    gnss_interface.set_impl(std::move(impl));

    ASSERT_EQ(gnss_interface.wait_for_data(mask, timeout), ReturnCode::RETURN_CODE_ILLEGAL_OPERATION);
}

TEST(GnssInterfaceTests, wait_for_dataError)
{
    GnssInterfaceTest gnss_interface;
    std::unique_ptr<GnssInterfaceImplMock> impl = std::make_unique<GnssInterfaceImplMock>();
    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();
    std::chrono::milliseconds timeout(123);

    EXPECT_CALL(*impl, wait_for_data(_, timeout))
        .WillOnce(Return(ReturnCode::RETURN_CODE_ERROR));

    gnss_interface.set_impl(std::move(impl));

    ASSERT_EQ(gnss_interface.wait_for_data(mask, timeout), ReturnCode::RETURN_CODE_ERROR);
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
