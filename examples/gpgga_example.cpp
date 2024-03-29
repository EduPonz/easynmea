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

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <csignal>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <easynmea/EasyNmea.hpp>

/* Example's global variables */
namespace eduponz {
namespace easynmea {
namespace gpgga_example {

std::condition_variable cv;  // For controlling when to exit the example in an ordered manner
std::mutex mtx;  // Associated with the condition variable
std::atomic_bool running;  // Flag to store whether the working thread should be running

//! Print the example's help guide
void print_help()
{
    std::cout << "------------------------" << std::endl;
    std::cout << "EasyNMEA - GPGGA Example" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "Usage: ./gpgga_example [OPTIONS]" << std::endl;
    std::cout << "    -h/--help:                Print this help and exit" << std::endl;
    std::cout << "    -b/--baudrate [bauds]:    The connection baud rate in bauds [Defaults: 9600]" << std::endl;
    std::cout << "    -p/--serial_port [port]:  The serial port to use [Defaults: '/dev/ttyACM0']" << std::endl;
    std::cout << "Example: ./gpgga_example -p /dev/ttyUSB0 -b 9600" << std::endl;
}

/**
 * Callback for SIGINT signal
 *
 * Sets the running flag to false and signals the example's condition variable
 *
 * @param signum The signal captured.
 */
void signal_handler_callback(
        int signum)
{
    std::cout << std::endl << "Caught signal " << signum << "; closing down..." << std::endl;
    running.store(false);
    cv.notify_one();
}

/**
 * Pretty print a GPGGAData sample
 *
 * @param gpgga_data The sample to print
 */
void print_gpgga(
        const GPGGAData& gpgga_data)
{
    std::cout << "GPGGA Data - GNSS Position Fix" << std::endl;
    std::cout << "==============================" << std::endl;
    std::cout << "Timestamp ------------------------> " << gpgga_data.timestamp << std::endl;
    std::cout << "Latitude -------------------------> " << gpgga_data.latitude << "º N" << std::endl;
    std::cout << "Longitude ------------------------> " << gpgga_data.longitude << "º E" << std::endl;
    std::cout << "Fix ------------------------------> " << gpgga_data.fix << std::endl;
    std::cout << "Number of satellites -------------> " << gpgga_data.satellites_on_view << std::endl;
    std::cout << "Horizontal dilution of precision -> " << gpgga_data.horizontal_precision << std::endl;
    std::cout << "Altitude -------------------------> " << gpgga_data.altitude << std::endl;
    std::cout << "Height of geoid ------------------> " << gpgga_data.height_of_geoid << std::endl;
    std::cout << "DGPS last update -----------------> " << gpgga_data.dgps_last_update << std::endl;
    std::cout << "DGPS reference station ID --------> " << gpgga_data.dgps_reference_station_id << std::endl;
}

//! Get UNIX epoch time in milliseconds
inline int64_t get_epoch()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).
                   count();
}

/**
 * The example's routine
 *
 * Waits for until GPGGA data is avaliable, then takes the received data, prints it alongside how
 * much time was the routine waiting, and waits again. If some error happens while waiting, the
 * routine exists setting the running flag to false and signalling the condition variable.
 *
 * @param easynmea A reference to the EasyNmea instance
 */
void working_routine(
        EasyNmea& easynmea)
{
    /* Variables to calculate how much time was the routine waiting */
    int64_t last_sample_time = get_epoch();
    int64_t sample_time;

    /* Placeholder for GPGGA data */
    GPGGAData gpgga_data;

    /* Wait until there is GPGGA data available */
    NMEA0183DataKindMask data_mask = NMEA0183DataKind::GPGGA;
    ReturnCode ret = easynmea.wait_for_data(data_mask);

    /* Loop until waiting does not return OK anymore */
    while (ret == ReturnCode::RETURN_CODE_OK)
    {
        /* Take and print all the available samples, and the waiting time */
        while (easynmea.take_next(gpgga_data) == ReturnCode::RETURN_CODE_OK)
        {
            sample_time = get_epoch();
            std::cout << std::endl;
            std::cout << "************** NEW GPGGA SAMPLE **************" << std::endl;
            std::cout << "Elapsed time ---------> " << sample_time - last_sample_time << std::endl;
            std::cout << "------------------------------------------" << std::endl;
            print_gpgga(gpgga_data);
            last_sample_time = get_epoch();
        }
        /* Wait until there is GPGGA data available */
        ret = easynmea.wait_for_data(data_mask);
    }

    /**
     * If wait_for_data returned an error, then set the running flag to false and signal the
     * condition variable
     */
    if (ret == ReturnCode::RETURN_CODE_ERROR)
    {
        if (running)
        {
            std::cout << "Something went wrong while waiting for data. Closing down" << std::endl;
            running.store(false);
        }
        cv.notify_one();
    }
}

} // namespace gpgga_example
} // namespace easynmea
} // namespace eduponz

using namespace eduponz::easynmea;
using namespace eduponz::easynmea::gpgga_example;

int main(
        int argc,
        char* argv[])
{
    /**
     * Example arguments:
     *    'serial_port' is the port used to established a serial connection with the NMEA device
     *    'baudrate' is the bit-transmission speed of the communication. It must be set to whatever
     *               the NMEA device is using.
     */
    std::string serial_port = "/dev/ttyACM0";
    uint32_t baudrate = 9600;

    /**
     * Parse command line arguments:
     *    -h/--help to print the example's help
     *    -b/--baudrate to specify the baud rate
     *    -p/--serial_port to specify the serial port
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
        else
        {
            std::cout << "Unknown option: '" << argv[i] << "'" << std::endl;
            print_help();
            return 1;
        }
    }

    /* Open the serial communication */
    EasyNmea easynmea;
    if (!easynmea.open(serial_port.c_str(), baudrate))
    {
        // If cannot open, print help and exit
        std::cout <<  "Cannot open serial port '" << serial_port << "' with baudrate: " << baudrate << "." << std::endl;
        print_help();
        return 1;
    }

    std::cout << "Serial port '" << serial_port
              << "' opened. Baudrate: " << baudrate << std::endl;
    std::cout << "Please press CTRL-C to stop the example" << std::endl;

    /**
     * Spawn the examples working thread.
     * The main thread is blocked until a condition variable is signaled either beacause the user
     * pressed CTRL-C, or because the working thread ended unexpectedly.
     */
    running.store(true);  // Set the running flag
    std::thread working_thread(&working_routine, std::ref(easynmea)); // Spawn the thread
    signal(SIGINT, signal_handler_callback);  // Set a signal handler to capture SIGINT

    /* Block the main thread until the running flag is set to false */
    std::unique_lock<std::mutex> lck(mtx);
    cv.wait(lck, [&]()
            {
                return !running.load();
            });

    /* Close the serial connection */
    easynmea.close();

    /* Wait for the working thread to join*/
    working_thread.join();

    /* Exit successfully */
    return 0;
}
