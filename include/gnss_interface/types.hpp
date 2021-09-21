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
 * @file types.hpp
 */

#ifndef _GNSS_INTERFACE_TYPES_HPP_
#define _GNSS_INTERFACE_TYPES_HPP_

#include "Bitmask.hpp"

namespace eduponz {
namespace gnss_interface {

/**
 * @enum NMEA0183DataKind
 *
 * @brief Holds all the supported NMEA 0183 sentences.
 */
enum class NMEA0183DataKind : int32_t
{
    //! Represents no valid data kind
    INVALID = 0,

    //! Global Positioning System Fix Data
    GPGGA = 1 << 0,
};

/**
 * @brief Bitmask of NMEA0183 datas.
 *
 * Values of NMEA0183DataKind can be combined with the '|' operator to build the mask:
 *
 * \code{.cpp}
 *     NMEA0183DataKindMask mask = NMEA0183DataKind::GPGGA | NMEA0183DataKind::INVALID;
 * \endcode
 *
 * \sa Bitmask
 */
using NMEA0183DataKindMask = Bitmask<NMEA0183DataKind>;

/**
 * @class ReturnCode
 *
 * @brief Provides understandable return codes for the different operations that the library
 *        performs.
 *
 * These return codes can be easily compared for applications to handle different scenarios.
 */
class ReturnCode
{
public:

    //! Internal \c ReturnCode enumeration
    enum
    {
        //! Operation succeeded
        RETURN_CODE_OK = 0,

        //! No data available
        RETURN_CODE_NO_DATA = 1,

        //! Operation timed out
        RETURN_CODE_TIMEOUT = 2,

        //! Bad input parameter to function call
        RETURN_CODE_BAD_PARAMETER = 3,

        //! The operation is illegal
        RETURN_CODE_ILLEGAL_OPERATION = 4,

        //! The operation is not yet supported
        RETURN_CODE_UNSUPPORTED = 5,

        //! The operation failed with an unexpected error
        RETURN_CODE_ERROR = 6
    };

    //! Default constructor; construct a \c ReturnCode with value \c ReturnCode::RETURN_CODE_OK
    ReturnCode()
        : value_(RETURN_CODE_OK)
    {
    }

    //! Construct a return code from an integer representing the enum value
    ReturnCode(
            uint32_t e)
    {
        value_ = e;
    }

    /**
     * Check whether a return code is equal to this one
     *
     * @param[in] c A constant reference to the return code to compare with this one
     *
     * @return true if equal; false otherwise
     */
    bool operator ==(
            const ReturnCode& c) const
    {
        return value_ == c.value_;
    }

    /**
     * Check whether a return code is different from this one
     *
     * @param[in] c A constant reference to the return code to compare with this one
     *
     * @return true if not equal; false otherwise
     */
    bool operator !=(
            const ReturnCode& c) const
    {
        return value_ != c.value_;
    }

    explicit operator bool() = delete;

    /**
     * Get the internal value of the \c ReturnCode
     *
     * @return This \c ReturnCode internal value
     */
    uint32_t operator ()() const
    {
        return value_;
    }

    /**
     * Check whether this \c ReturnCode is equal to \c ReturnCode::RETURN_CODE_OK
     *
     * @return true if this \c ReturnCode is different than \c ReturnCode::RETURN_CODE_OK ;
     *         false otherwise
     */
    bool operator !() const
    {
        return value_ != 0;
    }

private:

    uint32_t value_;
};

} // namespace gnss_interface
} // namespace eduponz

#endif //_GNSS_INTERFACE_TYPES_HPP_
