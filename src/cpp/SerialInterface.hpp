/*
 * File:   SerialInterface.h
 * Author: Terraneo Federico
 * Distributed under the Boost Software License, Version 1.0.
 *
 * Created on September 10, 2009, 12:12 PM
 */

#ifndef _GNSS_INTERFACE_SERIALINTERFACE_H
#define	_GNSS_INTERFACE_SERIALINTERFACE_H

#include <asio.hpp>

namespace eduponz
{
namespace gnss_interface
{

class SerialInterface
{
public:
    /**
     * Constructor.
     * \param port device name, example "/dev/ttyUSB0" or "COM4"
     * \param baud_rate communication speed, example 9600 or 115200
     * \throws system::system_error if cannot open the
     * serial device
     */
    SerialInterface(
            std::string port,
            unsigned int baud_rate)
        : io()
        , serial(io, port)
    {
        serial.set_option(asio::serial_port_base::baud_rate(baud_rate));
    }

    /**
     * Write a string to the serial device.
     * \param s string to write
     * \throws system::system_error on failure
     */
    void write(
            std::string s)
    {
        asio::write(serial,asio::buffer(s.c_str(), s.size()));
    }

    /**
     * Blocks until a line is received from the serial device.
     * Eventual '\n' or '\r\n' characters at the end of the string are removed.
     * \return a string containing the received line
     * \throws system::system_error on failure
     */
    std::string read_line()
    {
        // Reading data char by char, code is optimized for simplicity, not speed
        char c;
        std::string result;
        for(;;)
        {
            asio::read(serial, asio::buffer(&c, 1));
            switch(c)
            {
                case '\r':
                    break;
                case '\n':
                    return result;
                default:
                    result+=c;
            }
        }
    }

private:
    asio::io_service io;

    asio::serial_port serial;
};

} // namespace eduponz
} // namespace gnss_interface

#endif	/* _GNSS_INTERFACE_SERIALINTERFACE_H */
