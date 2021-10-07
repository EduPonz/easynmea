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

#include <easynmea/types.hpp>
#include <easynmea/EasyNmea.hpp>

#include "EasyNmeaImplMock.hpp"

using namespace eduponz::easynmea;

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class EasyNmeaTest : public EasyNmea
{
public:

    EasyNmeaTest()
        : EasyNmea()
    {}

    void set_impl(std::unique_ptr<EasyNmeaImplMock> impl)
    {
        impl_ = std::move(impl);
    }
};

TEST(EasyNmeaTests, openOk)
{
    const char* port = "port";
    long baudrate = 9600;

    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();

    EXPECT_CALL(*impl, open(port, baudrate))
        .WillOnce(Return(ReturnCode::RETURN_CODE_OK));

    easynmea.set_impl(std::move(impl));

    EXPECT_EQ(easynmea.open(port, baudrate), ReturnCode::RETURN_CODE_OK);
}

TEST(EasyNmeaTests, openError)
{
    const char* port = "port";
    long baudrate = 9600;

    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();

    EXPECT_CALL(*impl, open(port, baudrate))
        .WillOnce(Return(ReturnCode::RETURN_CODE_ERROR));

    easynmea.set_impl(std::move(impl));

    EXPECT_EQ(easynmea.open(port, baudrate), ReturnCode::RETURN_CODE_ERROR);
}

TEST(EasyNmeaTests, openIllegal)
{
    const char* port = "port";
    long baudrate = 9600;

    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();

    EXPECT_CALL(*impl, open(port, baudrate))
        .WillOnce(Return(ReturnCode::RETURN_CODE_ILLEGAL_OPERATION));

    easynmea.set_impl(std::move(impl));

    EXPECT_EQ(easynmea.open(port, baudrate), ReturnCode::RETURN_CODE_ILLEGAL_OPERATION);
}

TEST(EasyNmeaTests, is_openOpened)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();

    EXPECT_CALL(*impl, is_open)
        .WillOnce(Return(true));

    easynmea.set_impl(std::move(impl));

    EXPECT_EQ(easynmea.is_open(), true);
}

TEST(EasyNmeaTests, is_openClosed)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();

    EXPECT_CALL(*impl, is_open)
        .WillOnce(Return(false));

    easynmea.set_impl(std::move(impl));

    EXPECT_EQ(easynmea.is_open(), false);
}

TEST(EasyNmeaTests, closeOk)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();

    EXPECT_CALL(*impl, close)
        .WillOnce(Return(ReturnCode::RETURN_CODE_OK));

    easynmea.set_impl(std::move(impl));

    ASSERT_EQ(easynmea.close(), ReturnCode::RETURN_CODE_OK);
}

TEST(EasyNmeaTests, closeError)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();

    EXPECT_CALL(*impl, close)
        .WillOnce(Return(ReturnCode::RETURN_CODE_ERROR));

    easynmea.set_impl(std::move(impl));

    ASSERT_EQ(easynmea.close(), ReturnCode::RETURN_CODE_ERROR);
}

TEST(EasyNmeaTests, closeIllegal)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();

    EXPECT_CALL(*impl, close)
        .WillOnce(Return(ReturnCode::RETURN_CODE_ILLEGAL_OPERATION));

    easynmea.set_impl(std::move(impl));

    ASSERT_EQ(easynmea.close(), ReturnCode::RETURN_CODE_ILLEGAL_OPERATION);
}

TEST(EasyNmeaTests, take_nextOk)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();
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

    easynmea.set_impl(std::move(impl));

    ASSERT_EQ(easynmea.take_next(gpgga), ReturnCode::RETURN_CODE_OK);

    ASSERT_EQ(gpgga, gpgga_ret);
}

TEST(EasyNmeaTests, take_nextNoData)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();
    GPGGAData gpgga;

    EXPECT_CALL(*impl, take_next(gpgga))
        .WillOnce(Return(ReturnCode::RETURN_CODE_NO_DATA));

    easynmea.set_impl(std::move(impl));

    ASSERT_EQ(easynmea.take_next(gpgga), ReturnCode::RETURN_CODE_NO_DATA);

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

TEST(EasyNmeaTests, wait_for_dataOk)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();
    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();
    std::chrono::milliseconds timeout(123);

    EXPECT_CALL(*impl, wait_for_data(_, timeout))
        .WillOnce(Return(ReturnCode::RETURN_CODE_OK));

    easynmea.set_impl(std::move(impl));

    ASSERT_EQ(easynmea.wait_for_data(mask, timeout), ReturnCode::RETURN_CODE_OK);
}

TEST(EasyNmeaTests, wait_for_dataTimeout)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();
    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();
    std::chrono::milliseconds timeout(123);

    EXPECT_CALL(*impl, wait_for_data(_, timeout))
        .WillOnce(Return(ReturnCode::RETURN_CODE_TIMEOUT));

    easynmea.set_impl(std::move(impl));

    ASSERT_EQ(easynmea.wait_for_data(mask, timeout), ReturnCode::RETURN_CODE_TIMEOUT);
}

TEST(EasyNmeaTests, wait_for_dataTimeoutDefault)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();
    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();

    EXPECT_CALL(*impl, wait_for_data)
        .WillOnce(Return(ReturnCode::RETURN_CODE_TIMEOUT));

    easynmea.set_impl(std::move(impl));

    ASSERT_EQ(easynmea.wait_for_data(mask), ReturnCode::RETURN_CODE_TIMEOUT);
}

TEST(EasyNmeaTests, wait_for_dataIllegal)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();
    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();
    std::chrono::milliseconds timeout(123);

    EXPECT_CALL(*impl, wait_for_data(_, timeout))
        .WillOnce(Return(ReturnCode::RETURN_CODE_ILLEGAL_OPERATION));

    easynmea.set_impl(std::move(impl));

    ASSERT_EQ(easynmea.wait_for_data(mask, timeout), ReturnCode::RETURN_CODE_ILLEGAL_OPERATION);
}

TEST(EasyNmeaTests, wait_for_dataError)
{
    EasyNmeaTest easynmea;
    std::unique_ptr<EasyNmeaImplMock> impl = std::make_unique<EasyNmeaImplMock>();
    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();
    std::chrono::milliseconds timeout(123);

    EXPECT_CALL(*impl, wait_for_data(_, timeout))
        .WillOnce(Return(ReturnCode::RETURN_CODE_ERROR));

    easynmea.set_impl(std::move(impl));

    ASSERT_EQ(easynmea.wait_for_data(mask, timeout), ReturnCode::RETURN_CODE_ERROR);
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
