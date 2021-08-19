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
 * @file data.hpp
 */

#ifndef _GNSS_INTERFACE_DATA_HPP_
#define _GNSS_INTERFACE_DATA_HPP_

#include <string>

#include "types.hpp"

namespace eduponz
{
namespace gnss_interface
{

struct NMEA0183Data
{
    //! Default constructor. Empty-initializes the struct
    NMEA0183Data(
            NMEA0183DataKind data_kind = NMEA0183DataKind::INVALID)
        : message("")
        , kind(data_kind)
    {
    };

    //! The received message
    std::string message;

    //! The NMEA0183DataKind of the data
    NMEA0183DataKind kind;
};

struct GPGGAData : NMEA0183Data
{
    //! UTC time hhmmss.milliseconds
    float timestamp;

    //! Latitude in degrees referred to North

    float latitude;
    //! Longitude in degrees referred to East

    float longitude;

    /** GNSS Fix
     *    - 0: no fix
     *    - 1 -> fix
     *    - 2 -> dif fix
     */
    uint8_t fix;

    //! Number of satelites on view
    uint16_t satellites_on_view;

    //! GNSS horizontal precision expressed in meters
    float horizontal_precision;

    //! GNSS reported altitude over sea level expressed in meters
    float altitude;

    //! Default constructor. Empty-initializes the struct
    GPGGAData()
        : NMEA0183Data::NMEA0183Data(NMEA0183DataKind::GPGGA)
        , timestamp(0)
        , latitude (0)
        , longitude (0)
        , fix (0)
        , satellites_on_view (0)
        , horizontal_precision (0)
        , altitude (0)
    {
    };
};

} // namespace gnss_interface
} // namespace eduponz

#endif //_GNSS_INTERFACE_DATA_HPP_
