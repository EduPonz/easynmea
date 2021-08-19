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

namespace eduponz
{
namespace gnss_interface
{

class GnssInterfaceImpl
{
public:

    GnssInterfaceImpl();

    ~GnssInterfaceImpl();

    ReturnCode open(
            const char* serial_port,
            long baud_rate);

    ReturnCode close();

    ReturnCode take_next(
            GPGGAData& gpgga);

    ReturnCode wait_for_data(
            NMEA0183DataKindMask data_mask);

protected:

    const std::string POSITION_START_ = "$GPGGA";

    SerialInterface* serial_interface_;

    std::unique_ptr<std::thread> read_thread_;

    std::atomic<bool> routine_running_;

    std::atomic<bool> new_position_;

    std::mutex mutex_;

    std::condition_variable cv_;

    FixedSizeQueue<GPGGAData, 10> gpgga_data_queue_;

    std::vector<std::string> break_string_(
            const std::string& str,
            char separator);

    float parse_to_degrees_(
            const std::string& str);

    bool parse_raw_line_(
            const std::string& line);

    bool populate_position_(
            const std::string& position_line);

    std::vector<std::string> read_raw_lines_();

    void read_routine_();

};

} // namespace eduponz
} // namespace gnss_interface

#endif //_GNSS_INTERFACE_IMPL_HPP_
