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

#ifndef _EASYNMEA_DATA_HPP_
#define _EASYNMEA_DATA_HPP_

#include <string>

#include "types.hpp"

namespace eduponz {
namespace easynmea {


/**
 * \struct NMEA0183Data
 *
 * @brief Base struct for all NMEA 0183 Data types
 */
struct NMEA0183Data
{
    /**
     * Default constructor; it empty-initializes the struct
     *
     * @param[in] data_kind The \c NMEA0183DataKind of the data instance. Defaults to
     *        \c NMEA0183DataKind::INVALID
     */
    NMEA0183Data(
            NMEA0183DataKind data_kind = NMEA0183DataKind::INVALID) noexcept
        : kind(data_kind)
    {
    }

    //! Default virtual constructor
    virtual ~NMEA0183Data() = default;

    //! The NMEA0183DataKind of the data
    NMEA0183DataKind kind;

    /**
     * Check whether a \c NMEA0183Data is equal to this one
     *
     * @param[in] other A constant reference to the \c NMEA0183Data to compare with this one
     *
     * @return true if equal; false otherwise
     */
    inline bool operator ==(
            const NMEA0183Data& other) const noexcept
    {
        return (kind == other.kind);
    }

    /**
     * Check whether a \c NMEA0183Data is different from this one
     *
     * @param[in] other A constant reference to the \c NMEA0183Data to compare with this one
     *
     * @return true if different; false otherwise
     */
    inline bool operator !=(
            const NMEA0183Data& other) const noexcept
    {
        return !(*this == other);
    }

};

/**
 * \struct GPGGAData
 *
 * @brief Struct for data from GPGGA sentences
 */
struct GPGGAData : NMEA0183Data
{
    /**
     *  Default constructor; it empty-initializes the struct, setting \c kind to
     * \c NMEA0183DataKind::GPGGA
     */
    GPGGAData() noexcept
        : NMEA0183Data::NMEA0183Data(NMEA0183DataKind::GPGGA)
        , timestamp(0)
        , latitude(0)
        , longitude(0)
        , fix(0)
        , satellites_on_view(0)
        , horizontal_precision(0)
        , altitude(0)
        , height_of_geoid(0)
        , dgps_last_update(-1)
        , dgps_reference_station_id(0)
    {
    }

    //! UTC time hhmmss.milliseconds
    float timestamp;

    //! Latitude in degrees referred to North

    float latitude;
    //! Longitude in degrees referred to East

    float longitude;

    /** GNSS Fix
     *    - 0: no fix
     *    - 1 -> fix
     *    - 2 -> differential fix
     */
    uint16_t fix;

    //! Number of satellites on view
    uint16_t satellites_on_view;

    //! GNSS horizontal precision expressed in meters
    float horizontal_precision;

    //! GNSS reported altitude over sea level expressed in meters
    float altitude;

    //! Height of geoid above WGS84 ellipsoid in meters
    float height_of_geoid;

    //! Seconds since last DGPS update
    float dgps_last_update;

    //! DGPS reference station ID
    uint16_t dgps_reference_station_id;

    /**
     * Check whether a \c GPGGAData is equal to this one
     *
     * @param[in] other A constant reference to the \c GPGGAData to compare with this one
     *
     * @return true if equal; false otherwise
     */
    inline bool operator ==(
            const GPGGAData& other) const noexcept
    {
        return (NMEA0183Data::operator ==(other) &&
               timestamp == other.timestamp &&
               latitude == other.latitude &&
               longitude == other.longitude &&
               fix == other.fix &&
               satellites_on_view == other.satellites_on_view &&
               horizontal_precision == other.horizontal_precision &&
               altitude == other.altitude &&
               height_of_geoid == other.height_of_geoid &&
               dgps_last_update == other.dgps_last_update &&
               dgps_reference_station_id == other.dgps_reference_station_id);
    }

    /**
     * Check whether a \c GPGGAData is different from this one
     *
     * @param[in] other A constant reference to the \c GPGGAData to compare with this one
     *
     * @return true if different; false otherwise
     */
    inline bool operator !=(
            const GPGGAData& other) const noexcept
    {
        return !(*this == other);
    }

};

} // namespace easynmea
} // namespace eduponz

#endif //_EASYNMEA_DATA_HPP_
