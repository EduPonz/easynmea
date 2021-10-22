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

#include "EasyNmeaCoder.hpp"
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
        gpgga = *gpgga_data_queue_.front();
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

bool EasyNmeaImpl::process_line_(
        const std::string& line) noexcept
{
    std::shared_ptr<NMEA0183Data> data = EasyNmeaCoder::decode(line);
    switch (data->kind)
    {
        case NMEA0183DataKind::GPGGA:
        {
            {
                std::unique_lock<std::mutex> lck(data_mutex_);
                gpgga_data_queue_.push(std::static_pointer_cast<GPGGAData>(data));
                data_received_.set(NMEA0183DataKind::GPGGA);
            }
            cv_.notify_all();
            return true;
        }
        default:
        {
            return false;
        }
    }
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
            process_line_(line);
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
