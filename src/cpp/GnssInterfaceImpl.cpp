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
{
}

GnssInterfaceImpl::~GnssInterfaceImpl()
{
    gpgga_data_queue_.clear();
    close();
}

ReturnCode GnssInterfaceImpl::open(
        const char* serial_port,
        long baud_rate)
{
    if (!serial_interface_)
    {
        serial_interface_ = new SerialInterface(serial_port, baud_rate);
        routine_running_.store(true);
        read_thread_.reset(new std::thread(&GnssInterfaceImpl::read_routine_, this));
        return ReturnCode::RETURN_CODE_OK;
    }
    return ReturnCode::RETURN_CODE_ILLEGAL_OPERATION;
}

ReturnCode GnssInterfaceImpl::close()
{
    if (serial_interface_)
    {
        routine_running_.store(false);
        if (read_thread_)
        {
            read_thread_->join();
            read_thread_ = nullptr;
        }
        delete serial_interface_;
        serial_interface_ = nullptr;
        // Break any wait_for_data
        cv_.notify_one();
        return ReturnCode::RETURN_CODE_OK;
    }
    return ReturnCode::RETURN_CODE_ILLEGAL_OPERATION;
}

ReturnCode GnssInterfaceImpl::take_next(
        GPGGAData& gpgga)
{
    if (!serial_interface_)
    {
        return ReturnCode::RETURN_CODE_ILLEGAL_OPERATION;
    }

    std::unique_lock<std::mutex> lck(mutex_);
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
    if (!serial_interface_)
    {
        return ReturnCode::RETURN_CODE_ILLEGAL_OPERATION;
    }

    std::unique_lock<std::mutex> lck(mutex_);
    cv_.wait(lck, [&]()
        {
            return !routine_running_.load() ||
                   (new_position_.load() && data_mask.is_set(NMEA0183DataKind::GPGGA));
        });

    if (routine_running_)
    {
        new_position_.store(false);
        return ReturnCode::RETURN_CODE_OK;
    }
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
    std::vector<std::string> content = break_string_(str, '.');

    std::string minutes = content[0].substr(content[0].size() - 2);
    minutes += "." + content[1];

    std::string::size_type idx;
    float minutes_float = std::stof(minutes, &idx);
    minutes_float = minutes_float / 60;

    content[0].erase(content[0].end() - 2, content[0].end());
    idx = 0;
    float degrees = std::stof(content[0], &idx) + minutes_float;

    return degrees;
}

bool GnssInterfaceImpl::parse_raw_line_(
        const std::string& line)
{
    if (std::strncmp(line.c_str(), POSITION_START_.c_str(), POSITION_START_.size()) == 0)
    {
        if (populate_position_(line))
        {
            new_position_.store(true);
            cv_.notify_one();
        }
    }
    return false;
}

bool GnssInterfaceImpl::populate_position_(
        const std::string& position_line)
{
    std::vector<std::string> content = break_string_(position_line, ',');

    std::unique_lock<std::mutex> lck(mutex_);
    GPGGAData gpgga_data;
    gpgga_data.kind = NMEA0183DataKind::GPGGA;
    if (content.size() >= 10)
    {
        std::string::size_type idx;

        gpgga_data.message = position_line;
        gpgga_data.timestamp = std::stof(content[1], &idx);
        idx = 0;

        gpgga_data.latitude = parse_to_degrees_(content[2]);
        gpgga_data.longitude = parse_to_degrees_(content[4]);
        if (content[3] != "N")
        {
            gpgga_data.latitude = -gpgga_data.latitude;
        }
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
    else
    {
        gpgga_data.fix = 0;
        return false;
    }
}

void GnssInterfaceImpl::read_routine_()
{
    while (routine_running_)
    {
        std::string line = serial_interface_->read_line();
        parse_raw_line_(line);
    }
}
