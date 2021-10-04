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
 * @file system_tests.cpp
 */

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <gnss_interface/GnssInterface.hpp>

/* Global variables */
namespace eduponz {
namespace gnss_interface {
namespace system_tests {

std::condition_variable cv;  // For controlling when to exit the example in an ordered manner
std::mutex mtx;  // Associated with the condition variable
std::atomic_bool running;  // Flag to store whether the working thread should be running
std::atomic<uint8_t> exit_code = {0};  // Programs exit code

//! Print the example's help guide
void print_help()
{
    std::cout << "-----------------------------" << std::endl;
    std::cout << "GNSS Interface - System Tests" << std::endl;
    std::cout << "-----------------------------" << std::endl;
    std::cout << "Usage: ./system_tests [OPTIONS]" << std::endl;
    std::cout << "    -h/--help:                Print this help and exit" << std::endl;
    std::cout << "    -b/--baudrate [bauds]:    The connection baud rate in bauds [Defaults: 9600]" << std::endl;
    std::cout << "    -p/--serial_port [port]:  The serial port to use [Defaults: '/dev/ttyACM0']" << std::endl;
    std::cout << "    -o/--output_file [filename]:  The output file [Defaults: './system_tests.out']" << std::endl;
    std::cout << "Example: ./system_tests -p /dev/ttyUSB0 -b 9600 -o system_tests.out" << std::endl;
}

/**
 * Callback for signal handling
 *
 * Sets the running flag to false and signals the example's condition variable
 *
 * @param signum The signal captured.
 */
void signal_handler_callback(
        int signum)
{
    std::cout << "Received signal: " << signum << std::endl;
    running.store(false);
    cv.notify_one();
}

/**
 * Log a GPGGAData sample
 *
 * @param gpgga_data The sample to log
 *
 * @return true if success; false otherwise
 */
bool log_data(
        const GPGGAData& gpgga_data,
        const std::string& file_name)
{
    // Open file to append
    std::ofstream log_file(file_name, std::ios_base::app);
    if (log_file.is_open())
    {
        log_file << "  - nmea_type: GPGGA" << std::endl;
        log_file << "    sentence: " << gpgga_data.message << std::endl;
        log_file << "    latitude: " << gpgga_data.latitude << std::endl;
        log_file << "    longitude: " << gpgga_data.longitude << std::endl;
        log_file << "    fix: " << gpgga_data.fix << std::endl;
        log_file << "    number_of_satellites: " << gpgga_data.satellites_on_view << std::endl;
        log_file << "    horizontal_precision: " << gpgga_data.horizontal_precision << std::endl;
        log_file << "    altitude: " << gpgga_data.altitude << std::endl;
        log_file.close();
        return true;
    }
    std::cout << "Cannot open output file " << std::endl;
    return false;
}

/**
 * The example's routine
 *
 * Waits for until GPGGA data is avaliable, then takes the received data, logs it and waits again.
 * If some error happens while waiting, the routine exists setting the running flag to false and
 * signalling the condition variable.
 *
 * @param gnss A reference to the GNSSInterface instance
 */
void working_routine(
        GnssInterface& gnss,
        const std::string& output_file)
{
    /* Variables to calculate how much time was the routine waiting */
    int64_t sample_time;

    /* Placeholder for GPGGA data */
    GPGGAData gpgga_data;

    /* Wait until there is GPGGA data available */
    NMEA0183DataKindMask data_mask = NMEA0183DataKind::GPGGA;
    ReturnCode ret = gnss.wait_for_data(data_mask);

    /* Loop until waiting does not return OK anymore */
    while (ret == ReturnCode::RETURN_CODE_OK)
    {
        /* Take and print all the available samples, and the waiting time */
        while (gnss.take_next(gpgga_data) == ReturnCode::RETURN_CODE_OK)
        {
            if (!log_data(gpgga_data, output_file))
            {
                running.store(false);
                cv.notify_one();
                exit_code.store(1);
                return;
            }
        }
        /* Wait until there is GPGGA data available */
        ret = gnss.wait_for_data(data_mask);
    }

    /**
     * If wait_for_data returned an error, then set the running flag to false and signal the
     * condition variable
     */
    if (ret == ReturnCode::RETURN_CODE_ERROR)
    {
        if (running)
        {
            running.store(false);
        }
        cv.notify_one();
    }
}

} // namespace system_tests
} // namespace gnss_interface
} // namespace eduponz

using namespace eduponz::gnss_interface;
using namespace eduponz::gnss_interface::system_tests;

int main(
        int argc,
        char* argv[])
{
    /**
     * Executable arguments:
     *    'serial_port' is the port used to established a serial connection with the GNSS device
     *    'baudrate' is the bit-transmission speed of the communication. It must be set to whatever
     *               the GNSS device is using.
     *    'output_file' is the file to which the received samples will be logged.
     */
    std::string serial_port = "/dev/ttyACM0";
    uint32_t baudrate = 9600;
    std::string output_file = "system_tests.out";

    /**
     * Parse command line arguments:
     *    -h/--help to print the example's help
     *    -b/--baudrate to specify the baud rate
     *    -p/--serial_port to specify the serial port
     *    -o/--output_file to specify the output file
     * Any unknown option causes the example to exit after printing the help guide
     */
    for (auto i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
        {
            print_help();
            return 0;
        }
        if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--baudrate") == 0)
        {
            baudrate = atoi(argv[++i]);
        }
        else if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--serial_port") == 0)
        {
            serial_port = argv[++i];
        }
        else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--output_file") == 0)
        {
            output_file = argv[++i];
        }
        else
        {
            std::cout << "Unknown option: '" << argv[i] << "'" << std::endl;
            print_help();
            return 1;
        }
    }

    /* Initialize output file */
    std::ofstream log_file(output_file);
    if (log_file.is_open())
    {
        log_file << "data:" << std::endl;
        log_file.close();
    }
    else
    {
        std::cout << "Cannot open output file " << output_file << std::endl;
        return 1;
    }

    /* Open the serial communication */
    GnssInterface gnss;
    if (!gnss.open(serial_port.c_str(), baudrate))
    {
        // If cannot open, print help and exit
        std::cout <<  "Cannot open serial port '" << serial_port << "' with baudrate: " << baudrate << "." << std::endl;
        print_help();
        return 1;
    }

    /**
     * Spawn the working thread.
     * The main thread is blocked until a condition variable is signaled either beacause the user
     * pressed CTRL-C, because a SIGTERM was captured, or because the working thread ended
     * unexpectedly.
     */
    running.store(true);  // Set the running flag
    std::thread working_thread(&working_routine, std::ref(gnss), std::ref(output_file)); // Spawn the thread
    signal(SIGINT, signal_handler_callback);  // Set a signal handler to capture SIGINT
    signal(SIGTERM, signal_handler_callback);  // Set a signal handler to capture SIGTERM

    /* Block the main thread until the running flag is set to false */
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck, [&]()
            {
                return !running.load();
            });

    /* Close the serial connection */
    gnss.close();

    /* Wait for the working thread to join*/
    working_thread.join();

    /* Exit with code */
    return exit_code.load();
}
