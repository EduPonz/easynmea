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

#include <chrono>

#include <opennmea/OpenNmea.hpp>
#include <opennmea/types.hpp>

#include "OpenNmeaImpl.hpp"
#include "SerialInterface.hpp"

using namespace eduponz::opennmea;

OpenNmea::OpenNmea() noexcept
    : impl_(std::make_unique<OpenNmeaImpl>())
{
}

OpenNmea::~OpenNmea() noexcept
{
}

ReturnCode OpenNmea::open(
        const char* serial_port,
        long baudrate) noexcept
{
    return impl_->open(serial_port, baudrate);
}

bool OpenNmea::is_open() noexcept
{
    return impl_->is_open();
}

ReturnCode OpenNmea::close() noexcept
{
    return impl_->close();
}

ReturnCode OpenNmea::take_next(
        GPGGAData& gpgga) noexcept
{
    return impl_->take_next(gpgga);
}

ReturnCode OpenNmea::wait_for_data(
        NMEA0183DataKindMask data_mask,
        std::chrono::milliseconds timeout) noexcept
{
    return impl_->wait_for_data(data_mask, timeout);
}
