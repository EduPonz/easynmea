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
    GnssInterfaceImpl();

    //! Destructor. Frees used memory and closes the serial connection if opened.
    ~GnssInterfaceImpl();

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
     *     * ReturnCode::RETURN_CODE_ILLEGAL_OPERATION if a previous call to open was performed in the
     *       same GnssInterface instance, regardless of the port.
     */
    ReturnCode open(
            const char* serial_port,
            long baudrate);

    /**
     * Check whether a serial connection is opened
     *
     * @return true if there is an opened serial connection; false otherwise.
     */
    bool is_open();

    /**
     * Close a serial connection
     *
     * \pre A successful call to \c open() has been performed.
     *
     * @return \c close() can return:
     *     * ReturnCode::RETURN_CODE_OK if the connection was successfully closed.
     *     * ReturnCode::RETURN_CODE_ILLEGAL_OPERATION if there was not open connection.
     */
    ReturnCode close();

    /**
     * \brief Take the next un-taken GPGGA data sample available
     *
     * \c GnssInterfaceImpl stores up to the last 10 reported GPGGA data samples. \c take_next() is
     * used to retrieve the oldest un-taken GPGGA sample.
     *
     * @param[out] gpgga A \c GPGGAData instance which will be populated with the sample.
     *
     * @return \c take_next() can return:
     *     * ReturnCode::RETURN_CODE_OK if the operation succeeded.
     *     * ReturnCode::RETURN_CODE_NO_DATA if there are not any un-taken \c GPGGAData samples.
     *     * ReturnCode::RETURN_CODE_ILLEGAL_OPERATION if there was not open connection.
     */
    ReturnCode take_next(
            GPGGAData& gpgga);

    /**
     * \brief Block the calling thread until there is data available
     *
     * Block the calling thread until data of the specified kind or kinds is available for the
     * taking.
     *
     * @param[in] data_mask A \c NMEA0183DataKindMask used to specified on which data kinds should
     *            the call return, thus unblocking the calling thread. Defaults to
     *            \c NMEA0183DataKindMask::all().
     *
     * @return \c wait_for_data() can return:
     *     * ReturnCode::RETURN_CODE_OK if a sample of any of the kinds specified in the mask has
     *       been received.
     *     * ReturnCode::RETURN_CODE_NO_DATA if some other thread called \c close() on the
     *       \c GnssInterfaceImpl instance, which unblocks any \c wait_for_data() calls.
     */
    ReturnCode wait_for_data(
            NMEA0183DataKindMask data_mask);

protected:

    //! The starting characters of a GPGGA sentence
    const std::string GPGGA_SENTENCE_START_ = "$GPGGA";

    //! Pointer to the seral interface to which the device is connected
    SerialInterface* serial_interface_;

    //! The thread in which the reading from the serial interface is performed
    std::unique_ptr<std::thread> read_thread_;

    //! Flag to indicate whether the reading routine is running
    std::atomic<bool> routine_running_;

    //! Flag to indicate that a new gpgga message has been received
    std::atomic<bool> new_position_;

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
            char separator);

    /**
     * Convert the NMEA 1082 string represention of an angle in degrees (DDMM.mmmm) into a float.
     *
     * @param str The string representing the angle
     *
     * @return The angle as a float
     */
    float parse_to_degrees_(
            const std::string& str);

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
            const std::string& line);

    /**
     * Take a GPGGA sentence, extract the GPGGA data from it, and push it to the collection.
     *
     * @param gpgga_sentence The GPGGA sentence
     *
     * @return true if the GPGGA data contained a fix position, false otherwise.
     */
    bool process_gpgga_(
            const std::string& gpgga_sentence);

    /**
     * Routine run by read_thread_.
     *
     * It reads incoming lines from the GNSS devices, parses them, and signals the \c cv_ when new
     * data arrives.
     *
     * TODO: This could just be a lambda
     */
    void read_routine_();

};

} // namespace eduponz
} // namespace gnss_interface

#endif //_GNSS_INTERFACE_IMPL_HPP_
