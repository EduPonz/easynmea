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
        if (!is_open())
        {
            asio::error_code ec;
            serial.open(port, ec);
            if (!ec)
            {
                serial.set_option(asio::serial_port_base::baud_rate(baudrate), ec);
                if (!ec)
                {
                    return true;
                }
            }
            std::cout << "[ERROR] Cannot open serial port '" << port << "'. Error: " << ec.message() << std::endl;
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
     * \pre The port was open
     * \return true if the port was open upon the call to \c close() and closed afterwards, or if
     *         the port was already closed upon the call to \c close(). false if the port was open
     *         and could not be closed.
     */
    bool close()
    {
        if (is_open())
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
     * Blocks until a line is received from the serial device.
     *
     * Eventual \c '\n' or \c '\r\n' characters at the end of the string are removed.
     *
     * \param[out] result A string to store the read line.
     * \return true if a line was read; false if the port was closed or there was an error while
     *         reading.
     */
    bool read_line(
            std::string& result)
    {
        if (!is_open())
        {
            return false;
        }
        // Reading data char by char, code is optimized for simplicity, not speed
        char c;
        result.clear();
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
