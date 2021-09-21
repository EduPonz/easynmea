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
 * @file GnssInterfaceImpl.hpp
 */

#ifndef _GNSS_INTERFACE_IMPL_HPP_
#define _GNSS_INTERFACE_IMPL_HPP_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <gnss_interface/GnssInterface.hpp>
#include <gnss_interface/types.hpp>

#include "FixedSizeQueue.hpp"
#include "SerialInterface.hpp"

using namespace std::chrono_literals;

namespace eduponz {
namespace gnss_interface {

/**
 * @class GnssInterfaceImpl
 *
 * This class provides the actual implementation of \c GnssInterface.
 */
class GnssInterfaceImpl
{
public:

    //! Default constructor. Constructs a \c GnssInterfaceImpl
    GnssInterfaceImpl() noexcept;

    //! Destructor. Frees used memory and closes the serial connection if opened.
    virtual ~GnssInterfaceImpl() noexcept;

    /**
     * \brief Open a serial connection.
     *
     * It opens a serial connection on a given port with a given baudrate; given that the connection
     * was not previously opened.
     *
     * \pre The GnssInterfaceImpl does not have any serial port opened. That is, either it is the
     * first call to \c open(), or \c close() has been called before \c open().
     *
     * @param[in] serial_port A string containing the serial port name.
     * @param[in] baudrate The communication baudrate.
     * @return \c open() can return:
     *     * ReturnCode::RETURN_CODE_OK if the port is opened correctly.
     *     * ReturnCode::RETURN_CODE_ERROR is the port could not be opened.
     *     * ReturnCode::RETURN_CODE_ILLEGAL_OPERATION if a previous call to open was performed in the
     *       same GnssInterface instance, regardless of the port.
     */
    virtual ReturnCode open(
            const char* serial_port,
            long baudrate) noexcept;

    /**
     * Check whether a serial connection is opened
     *
     * @return true if there is an opened serial connection; false otherwise.
     */
    virtual bool is_open() noexcept;

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
    virtual ReturnCode close() noexcept;

    /**
     * \brief Take the next untaken GPGGA data sample available
     *
     * \c GnssInterfaceImpl stores up to the last 10 reported GPGGA data samples. \c take_next() is
     * used to retrieve the oldest untaken GPGGA sample. If eventually \c take_next takes the last
     * gpgga sample received, then the corresponding bit of \c data_received_ is cleared.
     *
     * @param[out] gpgga A \c GPGGAData instance which will be populated with the sample.
     *
     * @return \c take_next() can return:
     *     * ReturnCode::RETURN_CODE_OK if the operation succeeded.
     *     * ReturnCode::RETURN_CODE_NO_DATA if there are not any untaken \c GPGGAData samples.
     */
    virtual ReturnCode take_next(
            GPGGAData& gpgga) noexcept;

    /**
     * \brief Block the calling thread until there is data available.
     *
     * Block the calling thread until data of the specified kind or kinds is available for the
     * taking, or the timeout expires.
     *
     * @param[in, out] data_mask A \c NMEA0183DataKindMask used to specify on which data kinds
     *            should the call return, thus unblocking the calling thread. When \c wait_for_data
     *            returns, data_mask holds the types of data that have been received.
     * @param[in] timeout The time in millisecond after which the function must return even when no
     *                    data was received
     *
     * @return \c wait_for_data() can return:
     *     * ReturnCode::RETURN_CODE_OK if a sample of any of the kinds specified in the mask has
     *       been received.
     *     * ReturnCode::RETURN_CODE_TIMEOUT if the timeout was reached without receiving any data
     *       sample of the kinds specified in the @param data_mask.
     *     * ReturnCode::RETURN_CODE_ILLEGAL_OPERATION if there was not open connection.
     *     * ReturnCode::RETURN_CODE_ERROR if some other thread called \c close() on the
     *       \c GnssInterfaceImpl instance, which unblocks any \c wait_for_data() calls.
     */
    virtual ReturnCode wait_for_data(
            NMEA0183DataKindMask data_mask,
            std::chrono::milliseconds timeout) noexcept;

protected:

    //! The starting characters of a GPGGA sentence
    const std::string GPGGA_SENTENCE_START_ = "$GPGGA";

    //! Pointer to the seral interface to which the device is connected
    SerialInterface<>* serial_interface_;

    //! The thread in which the reading from the serial interface is performed
    std::unique_ptr<std::thread> read_thread_;

    //! Flag to indicate whether the reading routine is running
    std::atomic<bool> routine_running_;

    //! \c NMEA0183DataKindMask to indicate which \c NMEA0183DataKind have been received and are yet to be taken
    NMEA0183DataKindMask data_received_;

    //! Flag to indicate whether an internal error has ocurred
    std::atomic<bool> internal_error_;

    //! Class mutex
    std::mutex mutex_;

    //! Data mutex. Intended to protect operations with the queue
    std::mutex data_mutex_;

    /**
     * Condition variable associated with GPGGA data.
     *
     * It is used on \c wait_for_data(), and it's signalled when a new GPGGA sample is received.
     */
    std::condition_variable cv_;

    //! Collection of the at most last ten GPGGA samples received from the device
    FixedSizeQueue<GPGGAData, 10> gpgga_data_queue_;

    /**
     * Break a string in substrings according to a separator character.
     *
     * @param str The string to break
     * @param separator The character to use as separator
     *
     * @return A vector containing the substrings. The separator is not included in the substrings.
     */
    std::vector<std::string> break_string_(
            const std::string& str,
            char separator) noexcept;

    /**
     * Convert the NMEA 1082 string represention of an angle in degrees (DDMM.mmmm) into a float.
     *
     * @param str The string representing the angle
     *
     * @return The angle as a float
     */
    float parse_to_degrees_(
            const std::string& str) noexcept;

    /**
     * Parse a NMEA 1082 sentence
     *
     * Currently, the only supported sentences are those of type GPGGA. This function is the entry
     * point of new lines read in the \c read_routine_(). It parses the line, adds the new gpgga
     * data to the vector, sets the \c new_position_ flag, and signals the condition variable.
     *
     * @param line The sentence to parse
     *
     * @return true on success, false otherwise.
     */
    bool parse_raw_line_(
            const std::string& line) noexcept;

    /**
     * Take a GPGGA sentence, extract the GPGGA data from it, and push it to the collection.
     *
     * @param gpgga_sentence The GPGGA sentence
     *
     * @return true if the GPGGA data contained a fix position, false otherwise.
     */
    bool process_gpgga_(
            const std::string& gpgga_sentence) noexcept;

    /**
     * Routine run by read_thread_.
     *
     * It reads incoming lines from the GNSS devices, parses them, and signals the \c cv_ when new
     * data arrives.
     *
     * TODO: This could just be a lambda
     */
    void read_routine_() noexcept;

    /**
     * Check whether a serial connection is opened. Mind that this function is not thread safe.
     *
     * @return true if there is an opened serial connection; false otherwise.
     */
    bool is_open_nts_() noexcept;

};

} // namespace eduponz
} // namespace gnss_interface

#endif //_GNSS_INTERFACE_IMPL_HPP_
