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
 * @file gpgga_example.cpp
 */

#include <chrono>
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include <gnss_interface/GnssInterface.hpp>

using namespace eduponz::gnss_interface;

void print_position(
        const GPGGAData& gpgga_data)
{
    std::cout << "POSITION"                 << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "Message --------------> " << gpgga_data.message              << std::endl;
    std::cout << "Timestamp ------------> " << gpgga_data.timestamp            << std::endl;
    std::cout << "Latitude -------------> " << gpgga_data.latitude             << std::endl;
    std::cout << "Longitude ------------> " << gpgga_data.longitude            << std::endl;
    std::cout << "Fix ------------------> " << gpgga_data.fix                  << std::endl;
    std::cout << "Number of satellites -> " << gpgga_data.satellites_on_view   << std::endl;
    std::cout << "Horizontal Precision -> " << gpgga_data.horizontal_precision << std::endl;
    std::cout << "Altitude -------------> " << gpgga_data.altitude             << std::endl;
}

int64_t get_epoch()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).
                   count();
}

void working_routine(GnssInterface& gnss)
{
    int64_t last_sample_time = get_epoch();
    int64_t sample_time = get_epoch();
    GPGGAData gpgga_data;

    while (gnss.wait_for_data() == ReturnCode::RETURN_CODE_OK)
    {
        while (gnss.take_next(gpgga_data) == ReturnCode::RETURN_CODE_OK)
        {
            sample_time = get_epoch();
            std::cout << std::endl;
            std::cout << "********************************** NEW POSITION **********************************" <<
                    std::endl;
            std::cout << "Elapsed time ---------> " << sample_time - last_sample_time << std::endl;
            std::cout << "----------------------------------------------------------------------------------" <<
                    std::endl;
            print_position(gpgga_data);
            last_sample_time = get_epoch();
        }
    }
}

int main()
{
    std::string serial_port = "/dev/ttyACM0";
    uint32_t baud_rate = 9600;

    GnssInterface gnss;
    std::cout << "GNSS Interface created!" << std::endl;

    if (!gnss.open(serial_port.c_str(), baud_rate))
    {
        std::cout <<  "Cannot open serial port '" << serial_port << "'." << std::endl;
        return 1;
    }

    std::cout << "Serial port '" << serial_port
        << "' opened. Baudrate: " << baud_rate << std::endl;

    std::cout << "Please press enter to stop the example" << std::endl;
    std::thread working_thread(&working_routine, std::ref(gnss));
    std::cin.ignore();
    gnss.close();
    working_thread.join();

    return 0;
}
