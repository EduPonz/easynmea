/*
 * File:   SerialInterface.h
 * Author: Terraneo Federico
 * Distributed under the Boost Software License, Version 1.0.
 *
 * Created on September 10, 2009, 12:12 PM
 */

#ifndef _GNSS_INTERFACE_SERIALINTERFACE_H
#define _GNSS_INTERFACE_SERIALINTERFACE_H

#include <asio.hpp>

namespace eduponz {
namespace gnss_interface {

class SerialInterface
{
public:

    /**
     * Constructor.
     */
    SerialInterface()
        : io()
        , serial(io)
    {
    }

    /**
     * Open the serial port
     *
     * \pre The communication was not already openned.
     *
     * \param port device name, example "/dev/ttyUSB0" or "COM4"
     * \param baudrate communication speed, example 9600 or 115200
     * \return true if it can open the communication; false otherwise.
     */
    bool open(
            std::string port,
            unsigned int baudrate)
    {
        if (!serial.is_open())
        {
            asio::error_code ec;
            serial.open(port, ec);
            if (!ec)
            {
                serial.set_option(asio::serial_port_base::baud_rate(baudrate));
                return true;
            }
            else
            {
                std::cout << "[ERROR] Cannot open serial port '" << port << "'. Error: "
                          << ec.message() << std::endl;
            }
        }
        return false;
    }

    bool is_open()
    {
        return serial.is_open();
    }

    /**
     * Close the serial port
     *
     * \pre The port was opened
     * \return true if the operation succeeded; false otherwise.
     */
    bool close()
    {
        if (serial.is_open())
        {
            asio::error_code ec;
            serial.close(ec);
            if (ec)
            {
                std::cout << "[ERROR] Cannot close serial port. Error: " << ec.message() << std::endl;
                return false;
            }
        }
        return true;
    }

    /**
     * Write a string to the serial device.
     * \param s string to write
     * \throws system::system_error on failure
     */
    void write(
            std::string s)
    {
        asio::write(serial, asio::buffer(s.c_str(), s.size()));
    }

    /**
     * Blocks until a line is received from the serial device.
     *
     * Eventual \c '\n' or \c '\r\n' characters at the end of the string are removed.
     *
     * \param[out] result A string to store the read line.
     * \return true if success; false otherwise.
     */
    bool read_line(
            std::string& result)
    {
        // Reading data char by char, code is optimized for simplicity, not speed
        char c;
        asio::error_code ec;
        while (true)
        {
            asio::read(serial, asio::buffer(&c, 1), ec);
            if (!ec)
            {
                switch (c)
                {
                    case '\r':
                        break;
                    case '\n':
                        return true;
                    default:
                        result += c;
                }
            }
            else
            {
                std::cout << "[ERROR] Cannot read character. Error: " << ec.message() << std::endl;
                return false;
            }
        }
    }

private:

    asio::io_service io;

    asio::serial_port serial;
};

} // namespace eduponz
} // namespace gnss_interface

#endif  /* _GNSS_INTERFACE_SERIALINTERFACE_H */
