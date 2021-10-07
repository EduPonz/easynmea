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
 * @file EasyNmeaImpl.hpp
 */

#include <chrono>

#include "EasyNmeaImpl.hpp"

#include <easynmea/types.hpp>

#include "SerialInterface.hpp"

using namespace eduponz::easynmea;
using namespace std::chrono_literals;

EasyNmeaImpl::EasyNmeaImpl() noexcept
    : serial_interface_(new SerialInterface<>())
    , read_thread_(nullptr)
    , routine_running_(false)
    , data_received_(NMEA0183DataKindMask::none())
    , internal_error_(false)
{
}

EasyNmeaImpl::~EasyNmeaImpl() noexcept
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
    delete serial_interface_;
    serial_interface_ = nullptr;
}

ReturnCode EasyNmeaImpl::open(
        const char* serial_port,
        long baudrate) noexcept
{
    std::unique_lock<std::mutex> lck(mutex_);
    /**
     * open() can only be called on non-opened interfaces. Calling open() in an already opened
     * interface is an illegal operation
     */
    if (!is_open_nts_())
    {
        if (serial_interface_->open(serial_port, baudrate))
        {
            // If the serial interface could be opened, then spawn the reading thread
            routine_running_.store(true);
            read_thread_.reset(new std::thread(&EasyNmeaImpl::read_routine_, this));
            internal_error_.store(false);
            return ReturnCode::RETURN_CODE_OK;
        }
        else
        {
            // If the serial interface could NOT be openned, then return error
            internal_error_.store(true);
            return ReturnCode::RETURN_CODE_ERROR;
        }
    }
    return ReturnCode::RETURN_CODE_ILLEGAL_OPERATION;
}

bool EasyNmeaImpl::is_open() noexcept
{
    std::unique_lock<std::mutex> lck(mutex_);
    return is_open_nts_();
}

ReturnCode EasyNmeaImpl::close() noexcept
{
    ReturnCode ret = ReturnCode::RETURN_CODE_ILLEGAL_OPERATION;
    std::unique_lock<std::mutex> lck(mutex_);
    /**
     * close() can only be called on opened interfaces. Calling close() on a non-opened interface is
     * an illegal operation
     */
    if (is_open_nts_())
    {
        routine_running_.store(false);
        // If close() succeeds, then return OK, else return ERROR
        ret = serial_interface_->close() ? ReturnCode::RETURN_CODE_OK : ReturnCode::RETURN_CODE_ERROR;
        read_thread_->join();
        read_thread_ = nullptr;
        // Break any wait_for_data
        lck.unlock();
        cv_.notify_all();
    }
    return ret;
}

ReturnCode EasyNmeaImpl::take_next(
        GPGGAData& gpgga) noexcept
{
    std::unique_lock<std::mutex> lck(data_mutex_);
    if (!gpgga_data_queue_.empty())
    {
        gpgga = gpgga_data_queue_.front();
        gpgga_data_queue_.pop();
        // If the take operation has emptied the queue, then update corresponding bit in data_received_
        if (gpgga_data_queue_.empty())
        {
            data_received_.clear(NMEA0183DataKind::GPGGA);
        }
        return ReturnCode::RETURN_CODE_OK;
    }
    return ReturnCode::RETURN_CODE_NO_DATA;
}

ReturnCode EasyNmeaImpl::wait_for_data(
        NMEA0183DataKindMask data_mask,
        std::chrono::milliseconds timeout) noexcept
{
    // Cannot wait if the connection is closed
    if (!is_open())
    {
        data_mask = NMEA0183DataKindMask::none();
        return ReturnCode::RETURN_CODE_ILLEGAL_OPERATION;
    }

    /**
     * Wait until either:
     *    1. The routine is not running
     *    2. There is a new sample of any of the data kinds in the mask
     *    3. Timeout is reached before any of the previous
     */
    std::unique_lock<std::mutex> lck(data_mutex_);
    bool did_timeout = !cv_.wait_for(lck, timeout, [&]()
                    {
                        return !routine_running_.load() ||
                        (data_mask.is_set(NMEA0183DataKind::GPGGA) && data_received_.is_set(NMEA0183DataKind::GPGGA));
                    });

    // If the wait timed out, return TIMEOUT
    if (did_timeout)
    {
        data_mask = NMEA0183DataKindMask::none();
        return ReturnCode::RETURN_CODE_TIMEOUT;
    }
    // If the data mask is not empty and there is received data
    else if (!data_mask.is_none() && !data_received_.is_none())
    {
        data_mask = data_received_;
        return ReturnCode::RETURN_CODE_OK;
    }
    // The wait did not time out, nor did it exit because there was data. Hence, an error happended.
    data_mask = NMEA0183DataKindMask::none();
    return ReturnCode::RETURN_CODE_ERROR;
}

std::vector<std::string> EasyNmeaImpl::break_string_(
        const std::string& str,
        char separator) noexcept
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

float EasyNmeaImpl::parse_to_degrees_(
        const std::string& str) noexcept
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

bool EasyNmeaImpl::parse_raw_line_(
        const std::string& line) noexcept
{
    // Check that the line is a GPGGA sentence
    if (std::strncmp(line.c_str(), GPGGA_SENTENCE_START_.c_str(), GPGGA_SENTENCE_START_.size()) == 0)
    {
        // If the sentence contained a new GPGGA with fix, then set the new_position_ flag and
        // signal the CV. This means that GPGGA sentences without a fixed position will not unblock
        // wait_for_data()
        if (process_gpgga_(line))
        {
            cv_.notify_all();
        }
    }
    return false;
}

bool EasyNmeaImpl::process_gpgga_(
        const std::string& gpgga_sentence) noexcept
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
        data_received_.set(NMEA0183DataKind::GPGGA);

        return true;
    }
    return false;
}

void EasyNmeaImpl::read_routine_() noexcept
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
        routine_running_.store(false);
        internal_error_.store(true);
        cv_.notify_all();
    }
}

bool EasyNmeaImpl::is_open_nts_() noexcept
{
    if (serial_interface_)
    {
        return serial_interface_->is_open();
    }
    return false;
}
