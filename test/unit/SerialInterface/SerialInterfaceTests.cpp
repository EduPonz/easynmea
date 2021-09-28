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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <SerialInterface.hpp>

#include "SerialPortMock.hpp"

using namespace eduponz::gnss_interface;

using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;

namespace eduponz
{
namespace gnss_interface
{

class SerialInterfaceTest : public SerialInterface<SerialPortMock>
{
public:

    /**
     * Set object used to manage the serial port
     *
     * IMPORTANT: This operation transfers the ownsership of @param serial to the
     * \c SerialInterfaceTest, which stores it in a \c std::unique_ptr<SerialPortMock>. This means
     * that whenever \c SerialInterfaceTest is destroyed, @param serial is destroyed with it, so it
     * CANNOT be destroyed anywhere else.
     *
     * @param serial Raw pointer to the \c SerialPortMock object.
     */
    void set_serial_port(SerialPortMock* serial)
    {
        serial_.reset(serial);
    }

    /**
     * Get the SerialInterfaceTest associated \c asio::io_service.
     *
     * @return A reference to the associated asio::io_service.
     */
    asio::io_service& io_service()
    {
        return io_service_;
    }

    /**
     * Read a character from the serial port
     *
     * This function either reads a char using the implementation of
     * SerialInterface<SerialPortMock>, or it returns the next character in msg_, moving the "next"
     * index (char_count_) one position to the right.
     * This is controlled with the \c use_parent_read_char_ flag.
     *
     * @param[out] c Reference to the char that will be populated with the read character
     * @param[out] ec Reference to the error_code returned that will be populated from the parent's
     *                implementation.
     * @return The number of read characters.
     */
    std::size_t read_char(char& c, asio::error_code& ec) noexcept override
    {
        if (use_parent_read_char_)
        {
            return SerialInterface<SerialPortMock>::read_char(c, ec);
        }
        c = msg_[char_count_++];
        return 1;
    }

    /**
     * Set the message that will be read character by character with \c read_char if the
     * \c use_parent_read_char_ is set to \c false.
     *
     * Setting a message sets \c char_count_ the "next character" index to 0.
     *
     * @param msg The string to set a read message
     */
    void set_msg(std::string msg)
    {
        char_count_ = 0;
        msg_ = msg;
    }

    /**
     * Set whether \c read_char should use the parent's implementation or not.
     */
    void use_parent_read_char(bool should_use = true)
    {
        use_parent_read_char_ = should_use;
    }

private:

    //! Message to be read character by character if \c use_parent_read_char_ is set to \c false
    std::string msg_;

    //! The index to the next character of \c msg_ to be read.
    uint8_t char_count_ = 0;

    /**
     * Flag to indicate whether \c read_char should call the parent's \c read_char, or should read
     * a character from \c msg instead.
     */
    bool use_parent_read_char_ = true;
};

}
}

TEST(SerialInterfaceTests, openSuccess)
{
    /**
     * Create the SerialInterface using the derived class, which specializes SerialInterface in
     * SerialPortMock
     */
    SerialInterfaceTest serial;

    /**
     * Create a SerialPortMock that will substitute the one that SerialInterfaceTest creates.
     * This way, we can set expectations on the SerialPortMock, thus mocking asio::serial_port
     * to check that the SerialInterface code handles all returns correctly.
     */
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());

    /* Implementation should not try to open a port without checking whether it's already opened */
    EXPECT_CALL(*serial_port_mock, is_open).WillOnce(Return(false));

    /**
     * SerialPortMock open() is expected to be called with the appropriate string. It should also
     * return an OK error_code
     */
    std::string serial_port_str = "/dev/ttyUSB0";
    asio::error_code ec_1;
    EXPECT_CALL(*serial_port_mock, open(serial_port_str, ec_1)).WillOnce(SetArgReferee<1>(asio::error_code()));

    /**
     * Set option should be called to set the baud rate. Unfortunately,
     * asio::serial_port_base::baud_rate does not offer a == operator, so we cannot check whether
     * it is called with 9600 bauds. Nonetheless, for this test, we expect set_option to return
     * true.
     */
    asio::error_code ec_2;
    EXPECT_CALL(*serial_port_mock, set_option)
        .WillOnce(DoAll(SetArgReferee<1>(asio::error_code()), Return(true)));

    /* Set the mock as the SerialInterfaceTest serial port handler */
    serial.set_serial_port(serial_port_mock);

    /* Open should return true in this case */
    ASSERT_TRUE(serial.open(serial_port_str, 9600));
}

TEST(SerialInterfaceTests, openOpened)
{
    /**
     * Create the SerialInterface using the derived class, which specializes SerialInterface in
     * SerialPortMock
     */
    SerialInterfaceTest serial;

    /**
     * Create a SerialPortMock that will substitute the one that SerialInterfaceTest creates.
     * This way, we can set expectations on the SerialPortMock, thus mocking asio::serial_port
     * to check that the SerialInterface code handles all returns correctly.
     */
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());

    /**
     * Implementation should not try to open a port without checking whether it's already opened.
     * For this test, this returns true, signifying that the port is already opened
     */
    EXPECT_CALL(*serial_port_mock, is_open).WillOnce(Return(true));

    /**
     * SerialPortMock open() should NOT be called
     */
    std::string serial_port_str = "/dev/ttyUSB0";
    EXPECT_CALL(*serial_port_mock, open).Times(0);

    /**
     * Set option should NOT be called to set the baud rate.
     */
    EXPECT_CALL(*serial_port_mock, set_option).Times(0);

    /* Set the mock as the SerialInterfaceTest serial port handler */
    serial.set_serial_port(serial_port_mock);

    /* Open should return false in this case */
    ASSERT_FALSE(serial.open(serial_port_str, 9600));
}

TEST(SerialInterfaceTests, openWrongPort)
{
    /**
     * Create the SerialInterface using the derived class, which specializes SerialInterface in
     * SerialPortMock
     */
    SerialInterfaceTest serial;

    /**
     * Create a SerialPortMock that will substitute the one that SerialInterfaceTest creates.
     * This way, we can set expectations on the SerialPortMock, thus mocking asio::serial_port
     * to check that the SerialInterface code handles all returns correctly.
     */
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());

    /* Implementation should not try to open a port without checking whether it's already opened */
    EXPECT_CALL(*serial_port_mock, is_open).WillOnce(Return(false));

    /**
     * SerialPortMock open() is expected to be called with the appropriate string. Since this test
     * is for when the port is not correct, it should also return a NOT OK error_code
     */
    std::string serial_port_str = "/dev/ttyUSB0";
    asio::error_code ec;
    EXPECT_CALL(*serial_port_mock, open(serial_port_str, ec)).WillOnce(SetArgReferee<1>(asio::error_code(25, asio::error::get_system_category())));

    /**
     * Set option should NOT be called to set the baud rate.
     */
    EXPECT_CALL(*serial_port_mock, set_option).Times(0);

    /* Set the mock as the SerialInterfaceTest serial port handler */
    serial.set_serial_port(serial_port_mock);

    /* Open should return false in this case */
    ASSERT_FALSE(serial.open(serial_port_str, 9600));
}

TEST(SerialInterfaceTests, openWrongBaudrate)
{
    /**
     * Create the SerialInterface using the derived class, which specializes SerialInterface in
     * SerialPortMock
     */
    SerialInterfaceTest serial;

    /**
     * Create a SerialPortMock that will substitute the one that SerialInterfaceTest creates.
     * This way, we can set expectations on the SerialPortMock, thus mocking asio::serial_port
     * to check that the SerialInterface code handles all returns correctly.
     */
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());

    /* Implementation should not try to open a port without checking whether it's already opened */
    EXPECT_CALL(*serial_port_mock, is_open).WillOnce(Return(false));

    /**
     * SerialPortMock open() is expected to be called with the appropriate string. It should also
     * return an OK error_code
     */
    std::string serial_port_str = "/dev/ttyUSB0";
    asio::error_code ec_1;
    EXPECT_CALL(*serial_port_mock, open(serial_port_str, ec_1)).WillOnce(SetArgReferee<1>(asio::error_code()));

    /**
     * Set option should be called to set the baud rate. In this case, it should return false an give and set and error code.
     */
    EXPECT_CALL(*serial_port_mock, set_option)
        .WillOnce(
            DoAll(
                SetArgReferee<1>(asio::error_code(25, asio::error::get_system_category())),
                Return(false)));

    /* Set the mock as the SerialInterfaceTest serial port handler */
    serial.set_serial_port(serial_port_mock);

    /* Open should return false in this case */
    ASSERT_FALSE(serial.open(serial_port_str, 9600));
}

TEST(SerialInterfaceTest, is_openOpened)
{
    SerialInterfaceTest serial;
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());
    EXPECT_CALL(*serial_port_mock, is_open).WillOnce(Return(true));
    serial.set_serial_port(serial_port_mock);
    ASSERT_TRUE(serial.is_open());
}

TEST(SerialInterfaceTest, is_openClosed)
{
    SerialInterfaceTest serial;
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());
    EXPECT_CALL(*serial_port_mock, is_open).WillOnce(Return(false));
    serial.set_serial_port(serial_port_mock);
    ASSERT_FALSE(serial.is_open());
}

TEST(SerialInterfaceTests, closeSuccess)
{
    SerialInterfaceTest serial;
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());
    EXPECT_CALL(*serial_port_mock, is_open).WillOnce(Return(true));
    EXPECT_CALL(*serial_port_mock, close)
        .WillOnce(SetArgReferee<0>(asio::error_code()));
    serial.set_serial_port(serial_port_mock);
    ASSERT_TRUE(serial.close());
}

TEST(SerialInterfaceTests, closeClosed)
{
    SerialInterfaceTest serial;
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());
    EXPECT_CALL(*serial_port_mock, is_open).WillOnce(Return(false));
    EXPECT_CALL(*serial_port_mock, close).Times(0);
    serial.set_serial_port(serial_port_mock);
    ASSERT_TRUE(serial.close());
}

TEST(SerialInterfaceTests, closeAsioError)
{
    SerialInterfaceTest serial;
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());
    EXPECT_CALL(*serial_port_mock, is_open).WillOnce(Return(true));
    asio::error_code ec;
    EXPECT_CALL(*serial_port_mock, close(ec))
        .WillOnce(SetArgReferee<0>(asio::error_code(25, asio::error::get_system_category())));
    serial.set_serial_port(serial_port_mock);
    ASSERT_FALSE(serial.close());
}

TEST(SerialInterfaceTests, read_lineSuccess)
{
    /* Create mock */
    SerialInterfaceTest serial;
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());

    /* Set test expectations */
    EXPECT_CALL(*serial_port_mock, is_open)
        .Times(2)
        .WillRepeatedly(Return(true));

    std::string msg = "hello\n";

    /* Set mock object */
    serial.set_msg(msg);
    serial.use_parent_read_char(false);
    serial.set_serial_port(serial_port_mock);

    /* Call with an empty string */
    std::string result;
    result.clear();
    ASSERT_TRUE(serial.read_line(result));
    ASSERT_TRUE(!result.empty());
    ASSERT_EQ(result, msg.substr(0, msg.size()-1));

    /**
     * Call with an non-empty string. Before attempting to read, read_line() should clear the
     * string, meaning that it is expected to be returned empty if the serial connection was opened.
     */
    msg = "here we are\r\n";
    result = "Some content";
    serial.set_msg(msg);
    ASSERT_TRUE(serial.read_line(result));
    ASSERT_TRUE(!result.empty());
    ASSERT_EQ(result, msg.substr(0, msg.size()-2));
}

TEST(SerialInterfaceTests, read_lineClosed)
{
    /* Create mock */
    SerialInterfaceTest serial;
    SerialPortMock* serial_port_mock = new SerialPortMock(serial.io_service());

    /* Set test expectations */
    EXPECT_CALL(*serial_port_mock, is_open)
        .Times(2)
        .WillRepeatedly(Return(false));
    EXPECT_CALL(*serial_port_mock, async_read_some).Times(0);

    /* Set mock object */
    serial.set_serial_port(serial_port_mock);

    /* Call with an empty string */
    std::string result;
    result.clear();
    ASSERT_FALSE(serial.read_line(result));
    ASSERT_TRUE(result.empty());

    /* Call with an non-empty string */
    result = "Some content";
    ASSERT_FALSE(serial.read_line(result));
    ASSERT_EQ(result, "Some content");
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
