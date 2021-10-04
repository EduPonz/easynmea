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

#include <thread>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <gnss_interface/types.hpp>
#include <GnssInterfaceImpl.hpp>

#include "SerialInterfaceMock.hpp"

using namespace eduponz::gnss_interface;

using ::testing::AnyNumber;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

class GnssInterfaceImplTest : public GnssInterfaceImpl
{
public:

    GnssInterfaceImplTest()
        : GnssInterfaceImpl()
    {
    }

    void set_serial_interface(
            SerialInterface<>* serial_interface)
    {
        delete serial_interface_;
        serial_interface_ = serial_interface;
    }

};

TEST(GnssInterfaceImplTests, openSuccess)
{
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .WillOnce(Return(false))
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, open)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, close)
    // .Times(AnyNumber())
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, read_line)
            .Times(AnyNumber())
            .WillRepeatedly(DoAll(SetArgReferee<0>("\n"), Return(true)));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.open("some_port", 12), ReturnCode::RETURN_CODE_OK);
}

TEST(GnssInterfaceImplTests, openOpened)
{
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .WillOnce(Return(true))
            .WillOnce(Return(false));

    EXPECT_CALL(*serial, open)
            .Times(0);

    EXPECT_CALL(*serial, close)
            .Times(AnyNumber());

    EXPECT_CALL(*serial, read_line)
            .Times(AnyNumber())
            .WillRepeatedly(SetArgReferee<0>("\n"));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.open("some_port", 12), ReturnCode::RETURN_CODE_ILLEGAL_OPERATION);
}

TEST(GnssInterfaceImplTests, openWrongPort)
{
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .Times(AnyNumber());

    EXPECT_CALL(*serial, open)
            .WillOnce(Return(false));

    EXPECT_CALL(*serial, close)
            .Times(AnyNumber());

    EXPECT_CALL(*serial, read_line)
            .Times(AnyNumber())
            .WillRepeatedly(SetArgReferee<0>("\n"));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.open("some_port", 12), ReturnCode::RETURN_CODE_ERROR);
}

TEST(GnssInterfaceImplTests, is_openOpened)
{
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .WillOnce(Return(true))
            .WillOnce(Return(false));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_TRUE(impl.is_open());
}

TEST(GnssInterfaceImplTests, is_openClosed)
{
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .WillRepeatedly(Return(false));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_FALSE(impl.is_open());

    impl.set_serial_interface(nullptr);

    ASSERT_FALSE(impl.is_open());
}

TEST(GnssInterfaceImplTests, closeSuccess)
{
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .WillOnce(Return(false))
            .WillOnce(Return(true))
            .WillOnce(Return(false));

    EXPECT_CALL(*serial, open)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, close)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, read_line)
            .Times(AnyNumber())
            .WillRepeatedly(SetArgReferee<0>("\n"));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.open("some_port", 12), ReturnCode::RETURN_CODE_OK);
    ASSERT_EQ(impl.close(), ReturnCode::RETURN_CODE_OK);
}

TEST(GnssInterfaceImplTests, closeError)
{
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .WillOnce(Return(false))
            .WillOnce(Return(true))
            .WillOnce(Return(false));

    EXPECT_CALL(*serial, open)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, close)
            .WillOnce(Return(false));

    EXPECT_CALL(*serial, read_line)
            .Times(AnyNumber())
            .WillRepeatedly(SetArgReferee<0>("\n"));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.open("some_port", 12), ReturnCode::RETURN_CODE_OK);
    ASSERT_EQ(impl.close(), ReturnCode::RETURN_CODE_ERROR);
}

TEST(GnssInterfaceImplTests, closeClosed)
{
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .WillRepeatedly(Return(false));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.close(), ReturnCode::RETURN_CODE_ILLEGAL_OPERATION);
}

TEST(GnssInterfaceImplTests, wait_for_dataData)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,,*46\n";
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .Times(AnyNumber())
            .WillOnce(Return(false))
            .WillOnce(Return(true))
            .WillOnce(Return(true))
            .WillRepeatedly(Return(false));

    EXPECT_CALL(*serial, open)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, close)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, read_line)
            .Times(AnyNumber())
            .WillRepeatedly(DoAll(SetArgReferee<0>(sentence), Return(true)));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.open("some_port", 12), ReturnCode::RETURN_CODE_OK);

    NMEA0183DataKindMask mask = NMEA0183DataKindMask::all();
    ASSERT_EQ(impl.wait_for_data(mask, 100ms), ReturnCode::RETURN_CODE_OK);

    ASSERT_TRUE(mask.is_set(NMEA0183DataKind::GPGGA));
}

TEST(GnssInterfaceImplTests, wait_for_dataClosed)
{
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .Times(AnyNumber())
            .WillRepeatedly(Return(false));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.wait_for_data(NMEA0183DataKindMask::all(), 100ms), ReturnCode::RETURN_CODE_ILLEGAL_OPERATION);
}

TEST(GnssInterfaceImplTests, wait_for_dataDataEmptyMask)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,,*46\n";
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .Times(AnyNumber())
            .WillOnce(Return(false))
            .WillOnce(Return(true))
            .WillOnce(Return(true))
            .WillOnce(Return(false));

    EXPECT_CALL(*serial, open)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, close)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, read_line)
            .Times(AnyNumber())
            .WillRepeatedly(DoAll(SetArgReferee<0>(sentence), Return(true)));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.open("some_port", 12), ReturnCode::RETURN_CODE_OK);

    NMEA0183DataKindMask mask = NMEA0183DataKindMask::none();
    ASSERT_EQ(impl.wait_for_data(mask, 100ms), ReturnCode::RETURN_CODE_TIMEOUT);

    ASSERT_TRUE(mask.is_none());

    ASSERT_EQ(impl.close(), ReturnCode::RETURN_CODE_OK);
}

TEST(GnssInterfaceImplTests, wait_for_dataError)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,,*46\n";
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .Times(AnyNumber())
            .WillOnce(Return(false))
            .WillOnce(Return(true))
            .WillOnce(Return(true))
            .WillOnce(Return(true))
            .WillOnce(Return(false));

    EXPECT_CALL(*serial, open)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, close)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, read_line)
            .Times(AnyNumber())
            .WillRepeatedly(DoAll(SetArgReferee<0>(sentence), Return(false)));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.open("some_port", 12), ReturnCode::RETURN_CODE_OK);

    NMEA0183DataKindMask mask = NMEA0183DataKindMask::none();
    ASSERT_EQ(impl.wait_for_data(mask, 100ms), ReturnCode::RETURN_CODE_ERROR);

    ASSERT_TRUE(mask.is_none());
}

TEST(GnssInterfaceImplTests, take_next)
{
    std::string sentence_1 = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,,*46\n";
    std::string sentence_2 = "$GPGGA,072705.000,5703.1740,S,00954.9459,W,1,7,1.97,-21.2,M,42.5,M,,*46\n";
    std::string sentence_3 = "$GPTXT,01,01,02,ANTSTATUS=OPEN*2B\n";
    std::string sentence_4 = "$GPGGA,072705.000\n";

    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .Times(AnyNumber())
            .WillOnce(Return(false))
            .WillOnce(Return(true))
            .WillOnce(Return(true))
            .WillOnce(Return(true))
            .WillOnce(Return(true))
            .WillOnce(Return(true))
            .WillRepeatedly(Return(false));

    EXPECT_CALL(*serial, open)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, close)
            .Times(AnyNumber());

    EXPECT_CALL(*serial, read_line)
            .Times(AnyNumber())
            .WillOnce(DoAll(SetArgReferee<0>(sentence_1), Return(true)))
            .WillOnce(DoAll(SetArgReferee<0>(sentence_2), Return(true)))
            .WillOnce(DoAll(SetArgReferee<0>(sentence_3), Return(true)))
            .WillRepeatedly(DoAll(SetArgReferee<0>(sentence_4), Return(true)));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.open("some_port", 12), ReturnCode::RETURN_CODE_OK);

    ASSERT_EQ(impl.wait_for_data(NMEA0183DataKindMask::all(), 100ms), ReturnCode::RETURN_CODE_OK);

    GPGGAData data;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    ASSERT_EQ(impl.take_next(data), ReturnCode::RETURN_CODE_OK);

    NMEA0183DataKindMask mask;
    mask.set(NMEA0183DataKind::GPGGA);
    ASSERT_EQ(impl.wait_for_data(mask, 100ms), ReturnCode::RETURN_CODE_OK);
    ASSERT_EQ(impl.take_next(data), ReturnCode::RETURN_CODE_OK);

    ASSERT_EQ(impl.wait_for_data(NMEA0183DataKindMask::all(), 100ms), ReturnCode::RETURN_CODE_TIMEOUT);
    ASSERT_EQ(impl.take_next(data), ReturnCode::RETURN_CODE_NO_DATA);

    ASSERT_EQ(impl.wait_for_data(NMEA0183DataKindMask::all(), 100ms), ReturnCode::RETURN_CODE_TIMEOUT);
    ASSERT_EQ(impl.take_next(data), ReturnCode::RETURN_CODE_NO_DATA);
}


TEST(GnssInterfaceImplTests, destroyNoClose)
{
    SerialInterfaceMock* serial = new SerialInterfaceMock();

    EXPECT_CALL(*serial, is_open)
            .WillOnce(Return(false))
            .WillOnce(Return(true))
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, open)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, close)
            .WillOnce(Return(true));

    EXPECT_CALL(*serial, read_line)
            .Times(AnyNumber())
            .WillRepeatedly(SetArgReferee<0>("\n"));

    GnssInterfaceImplTest impl;
    impl.set_serial_interface(serial);

    ASSERT_EQ(impl.open("some_port", 12), ReturnCode::RETURN_CODE_OK);
    ASSERT_NE(impl.wait_for_data(NMEA0183DataKindMask::all(), 100ms), ReturnCode::RETURN_CODE_OK);
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
