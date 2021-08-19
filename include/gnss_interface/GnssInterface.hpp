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

/**
 * @file GnssInterface.hpp
 */

#ifndef _GNSS_INTERFACE_HPP_
#define _GNSS_INTERFACE_HPP_

#include "Bitmask.hpp"
#include "data.hpp"
#include "types.hpp"

namespace eduponz
{
namespace gnss_interface
{

class GnssInterfaceImpl;

class GnssInterface
{
public:

    GnssInterface();

    ~GnssInterface();

    ReturnCode open(
        const char* serial_port,
        long baud_rate);

    ReturnCode close();

    ReturnCode take_next(
        GPGGAData& gpgga);

    ReturnCode wait_for_data(
        NMEA0183DataKindMask data_mask = NMEA0183DataKindMask::all());

private:

    GnssInterfaceImpl* impl_;

};

} // namespace gnss_interface
} // namespace eduponz

#endif //_GNSS_INTERFACE_HPP_
