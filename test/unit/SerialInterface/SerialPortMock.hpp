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

#include <asio/buffer.hpp>
#include <asio/error_code.hpp>
#include <asio/io_service.hpp>
#include <asio/serial_port_base.hpp>

class SerialPortMock
{
public:
    SerialPortMock(asio::io_service& io_service)
    {
        static_cast<void>(io_service);
    }

    MOCK_METHOD(void,
        open,
        (const std::string& device,
         asio::error_code& ec));

    MOCK_METHOD(bool,
        is_open,
        ());

    MOCK_METHOD(bool,
        set_option,
        (asio::serial_port_base::baud_rate baudrate,
         asio::error_code& ec));

    MOCK_METHOD(void,
        close,
        (asio::error_code& ec));

    MOCK_METHOD(std::size_t,
        read_some,
        (const asio::mutable_buffers_1& buffers,
         asio::error_code& ec));
};
