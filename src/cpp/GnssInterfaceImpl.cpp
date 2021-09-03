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

#include "GnssInterfaceImpl.hpp"

#include <gnss_interface/types.hpp>

#include "SerialInterface.hpp"

using namespace eduponz::gnss_interface;

GnssInterfaceImpl::GnssInterfaceImpl()
    : serial_interface_(nullptr)
    , read_thread_(nullptr)
    , routine_running_(false)
    , new_position_(false)
    , internal_error_(false)
{
}

GnssInterfaceImpl::~GnssInterfaceImpl()
{
    gpgga_data_queue_.clear();
    /**
     *  If close returns something other than OK, it means that the serial interface was already
     * closed. In that case, if the reading thread is still operative, then we need to wait until
     * it joins.
     */
    if (close() != ReturnCode::RETURN_CODE_OK)
    {
        if (read_thread_)
        {
            read_thread_->join();
            read_thread_ = nullptr;
        }
    }
}

ReturnCode GnssInterfaceImpl::open(
        const char* serial_port,
        long baudrate)
{
    std::unique_lock<std::mutex> lck(mutex_);
    /**
     * open() can only be called on non-opened interfaces. Calling open() in an already opened
     * interface is an illegal operation
     */
    if (!serial_interface_)
    {
        serial_interface_ = new SerialInterface();
        if (serial_interface_->open(serial_port, baudrate))
        {
            // If the serial interface could be opened, then spawn the reading thread
            routine_running_.store(true);
            read_thread_.reset(new std::thread(&GnssInterfaceImpl::read_routine_, this));
            internal_error_.store(false);
            return ReturnCode::RETURN_CODE_OK;
        }
        else
        {
            // If the serial interface could NOT be openned, then clean up and return error
            delete serial_interface_;
            serial_interface_ = nullptr;
            internal_error_.store(true);
            return ReturnCode::RETURN_CODE_ERROR;
        }
    }
    return ReturnCode::RETURN_CODE_ILLEGAL_OPERATION;
}

bool GnssInterfaceImpl::is_open()
{
    std::unique_lock<std::mutex> lck(mutex_);
    if (serial_interface_)
    {
        return serial_interface_->is_open();
    }
    return false;
}

ReturnCode GnssInterfaceImpl::close()
{
    std::unique_lock<std::mutex> lck(mutex_);
    /**
     * close() can only be called on opened interfaces. Calling close() in a non-opened interface is
     * an illegal operation
     */
    if (serial_interface_)
    {
        routine_running_.store(false);
        /**
         * If the call to close() came from a thread other than the reading thread, then we can wait
         * for the reading thread to join. It the reading thread is the one calling close(), then it
         * cannot wait on a join of itself.
         */
        if (read_thread_->get_id() != std::this_thread::get_id())
        {
            read_thread_->join();
            read_thread_ = nullptr;
        }
        serial_interface_->close();
        delete serial_interface_;
        serial_interface_ = nullptr;
        // Break any wait_for_data
        lck.unlock();
        cv_.notify_one();
        return ReturnCode::RETURN_CODE_OK;
    }
    return ReturnCode::RETURN_CODE_ILLEGAL_OPERATION;
}

ReturnCode GnssInterfaceImpl::take_next(
        GPGGAData& gpgga)
{
    std::unique_lock<std::mutex> lck(data_mutex_);
    if (!gpgga_data_queue_.empty())
    {
        gpgga = gpgga_data_queue_.front();
        gpgga_data_queue_.pop();
        return ReturnCode::RETURN_CODE_OK;
    }
    return ReturnCode::RETURN_CODE_NO_DATA;
}

ReturnCode GnssInterfaceImpl::wait_for_data(
        NMEA0183DataKindMask data_mask)
{
    // Cannot wait if the connection is closed
    if (!is_open())
    {
        return ReturnCode::RETURN_CODE_ILLEGAL_OPERATION;
    }

    /**
     * Wait until either:
     *    1. The routine is not running
     *    2. There is a new sample of any of the data kinds in the mask
     */
    std::unique_lock<std::mutex> lck(data_mutex_);
    cv_.wait(lck, [&]()
            {
                return !routine_running_.load() ||
                (data_mask.is_set(NMEA0183DataKind::GPGGA) && new_position_.load());
            });

    // Check whether an internal error ocurred while waiting
    if (internal_error_)
    {
        return ReturnCode::RETURN_CODE_ERROR;
    }
    // If the routine is still running after the wait, then there wasn't any close while waiting
    else if (routine_running_)
    {
        new_position_.store(false);
        return ReturnCode::RETURN_CODE_OK;
    }
    // If by the time the wait finished the routine was not running, then close() was called from
    // elsewhere, which unblocks the wait. In that case, there is no data to be returned
    return ReturnCode::RETURN_CODE_NO_DATA;
}

std::vector<std::string> GnssInterfaceImpl::break_string_(
        const std::string& str,
        char separator)
{
    std::string result = "";
    std::stringstream ss(str);
    std::string substr;
    std::vector<std::string> content;

    while (ss.good())
    {
        getline(ss, substr, separator);
        if (substr != "")
        {
            content.push_back(substr);
        }
    }

    return content;
}

float GnssInterfaceImpl::parse_to_degrees_(
        const std::string& str)
{
    /**
     * The expected format of str is DDMM.mmmm, where:
     *    1. DD corresponds to degrees
     *    1. MM.mmmm is a float representing minutes of degree
     */

    // Break the string into DDMM and mmmm
    std::vector<std::string> content = break_string_(str, '.');

    // Build a string MM.mmmm
    std::string minutes = content[0].substr(content[0].size() - 2);
    minutes += "." + content[1];

    // Convert the MM.mmmm into a floating point number in degrees
    std::string::size_type idx;
    float minutes_float = std::stof(minutes, &idx);
    minutes_float = minutes_float / 60;

    // Remove the MM part from DDMM
    content[0].erase(content[0].end() - 2, content[0].end());
    idx = 0;
    // Return the degrees as the summation of DD plus the MM.mmmm translated into degrees
    return std::stof(content[0], &idx) + minutes_float;
}

bool GnssInterfaceImpl::parse_raw_line_(
        const std::string& line)
{
    // Check that the line is a GPGGA sentence
    if (std::strncmp(line.c_str(), GPGGA_SENTENCE_START_.c_str(), GPGGA_SENTENCE_START_.size()) == 0)
    {
        // If the sentence contained a new GPGGA with fix, then set the new_position_ flag and
        // signal the CV. This means that GPGGA sentences without a fixed position will not unblock
        // wait_for_data()
        if (process_gpgga_(line))
        {
            new_position_.store(true);
            cv_.notify_one();
        }
    }
    return false;
}

bool GnssInterfaceImpl::process_gpgga_(
        const std::string& gpgga_sentence)
{
    // Separate all the entries of the GPGGA sentence
    std::vector<std::string> content = break_string_(gpgga_sentence, ',');

    std::unique_lock<std::mutex> lck(data_mutex_);
    // A complete GPGGA sentence with a fix has at least 10 elements
    if (content.size() >= 10)
    {
        GPGGAData gpgga_data;
        std::string::size_type idx;

        gpgga_data.message = gpgga_sentence;
        gpgga_data.timestamp = std::stof(content[1], &idx);
        idx = 0;

        gpgga_data.latitude = parse_to_degrees_(content[2]);
        gpgga_data.longitude = parse_to_degrees_(content[4]);
        // Always return latitude bearing North
        if (content[3] != "N")
        {
            gpgga_data.latitude = -gpgga_data.latitude;
        }
        // Always return longitude bearing East
        if (content[5] != "E")
        {
            gpgga_data.longitude = -gpgga_data.longitude;
        }

        gpgga_data.fix = std::stoi(content[6], &idx);
        idx = 0;
        gpgga_data.satellites_on_view = std::stoi(content[7], &idx);
        idx = 0;
        gpgga_data.horizontal_precision = std::stof(content[8], &idx);
        idx = 0;
        gpgga_data.altitude = std::stof(content[9], &idx);
        gpgga_data_queue_.push(gpgga_data);

        return true;
    }
    return false;
}

void GnssInterfaceImpl::read_routine_()
{
    // Execute until the flag says otherwise
    while (routine_running_)
    {
        std::string line;
        // Wait for a new line coming from the device
        if (serial_interface_->read_line(line))
        {
            parse_raw_line_(line);
            continue;
        }
        // TODO: Decide what to do if SerialInterface::read_line() fails, for now we close down the
        // GnssInterface
        routine_running_.store(false);
        internal_error_.store(true);
        close();
    }
}
