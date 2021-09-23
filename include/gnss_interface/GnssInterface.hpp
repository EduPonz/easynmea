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

#include <chrono>
#include <memory>

#include "Bitmask.hpp"
#include "data.hpp"
#include "types.hpp"

namespace eduponz {
namespace gnss_interface {

class GnssInterfaceImpl;

/**
 * @class GnssInterface
 *
 * @brief This class provides an interface with GNSS modules using NMEA 0183 protocol over serial
 * connections.
 *
 * It can be used to:
 *     * Open and close serial connection with the modules.
 *     * Wait for specific NMEA sentences to be received.
 *     * Read incoming GNSS data in a parsed and understandable manner.
 */
class GnssInterface
{
public:

    //! Default constructor. Constructs a \c GnssInterface
    GnssInterface() noexcept;

    //! Virtual default destructor.
    virtual ~GnssInterface() noexcept;

    /**
     * \brief Open a serial connection.
     *
     * It opens a serial connection on a given port with a given baudrate; given that the connection
     * was not previously opened.
     *
     * \pre The GnssInterface does not have any serial port opened. That is, either it is the first
     *      call to \c open(), or \c close() has been called before \c open().
     *
     * @param[in] serial_port A string containing the serial port name.
     * @param[in] baudrate The communication baudrate.
     * @return \c open() can return:
     *     * ReturnCode::RETURN_CODE_OK if the port is opened correctly.
     *     * ReturnCode::RETURN_CODE_ERROR is the port could not be opened.
     *     * ReturnCode::RETURN_CODE_ILLEGAL_OPERATION if a previous call to open was performed in the
     *       same GnssInterface instance, regardless of the port.
     */
    ReturnCode open(
            const char* serial_port,
            long baudrate) noexcept;

    /**
     * Check whether a serial connection is opened
     *
     * @return true if there is an opened serial connection; false otherwise.
     */
    bool is_open() noexcept;

    /**
     * Close a serial connection
     *
     * \pre A successful call to \c open() has been performed.
     *
     * @return \c close() can return:
     *     * ReturnCode::RETURN_CODE_OK if the connection was successfully closed.
     *     * ReturnCode::RETURN_CODE_ERROR if the connection could not be closed.
     *     * ReturnCode::RETURN_CODE_ILLEGAL_OPERATION if there was not open connection.
     */
    ReturnCode close() noexcept;

    /**
     * \brief Take the next untaken GPGGA data sample available.
     *
     * \c GnssInterface stores up to the last 10 reported GPGGA data samples. \c take_next() is used
     * to retrieve the oldest untaken GPGGA sample.
     *
     * @param[out] gpgga A \c GPGGAData instance which will be populated with the sample.
     *
     * @return \c take_next() can return:
     *     * ReturnCode::RETURN_CODE_OK if the operation succeeded.
     *     * ReturnCode::RETURN_CODE_NO_DATA if there are not any untaken \c GPGGAData samples.
     */
    ReturnCode take_next(
            GPGGAData& gpgga) noexcept;

    /**
     * \brief Block the calling thread until there is data available.
     *
     * Block the calling thread until data of the specified kind or kinds is available for the
     * taking, or the timeout expires.
     *
     * @param[in] data_mask A \c NMEA0183DataKindMask used to specify on which data kinds should
     *            the call return, thus unblocking the calling thread. When \c wait_for_data returns
     *            data_mask holds the types of data that have been received. Defaults to
     *            \c NMEA0183DataKindMask::all().
     * @param[in] timeout The time in millisecond after which the function must return even when no
     *                    data was received. Defaults to 8760 hours (1 year).
     *
     * @return \c wait_for_data() can return:
     *     * ReturnCode::RETURN_CODE_OK if a sample of any of the kinds specified in the mask has
     *       been received.
     *     * ReturnCode::RETURN_CODE_TIMEOUT if the timeout was reached without receiving any data
     *       sample of the kinds specified in the \c data_mask.
     *     * ReturnCode::RETURN_CODE_ILLEGAL_OPERATION if there was not open connection.
     *     * ReturnCode::RETURN_CODE_ERROR if some other thread called \c close() on the
     *       \c GnssInterface instance, which unblocks any \c wait_for_data() calls.
     */
    ReturnCode wait_for_data(
            NMEA0183DataKindMask data_mask = NMEA0183DataKindMask::all(),
            std::chrono::milliseconds timeout = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::hours(
                8760))) noexcept;

protected:

    //! Unique pointer to the internal \c GnssInterfaceImpl object.
    std::unique_ptr<GnssInterfaceImpl> impl_;

};

} // namespace gnss_interface
} // namespace eduponz

#endif //_GNSS_INTERFACE_HPP_
