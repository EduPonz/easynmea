/*
 * File:   SerialInterface.h
 * Author: Terraneo Federico
 * Distributed under the Boost Software License, Version 1.0.
 *
 * Created on September 10, 2009, 12:12 PM
 */

#ifndef _OPENNMEA_SERIALINTERFACE_H
#define _OPENNMEA_SERIALINTERFACE_H

#include <functional>
#include <memory>

#include <asio/io_service.hpp>
#include <asio/read.hpp>
#include <asio/serial_port_base.hpp>
#include <asio/serial_port.hpp>

namespace eduponz {
namespace opennmea {

/**
 * @class SerialInterface
 *
 * This template class provides API to open and close a serial port, as well as for reading a line
 * from it (terminated either in \c '\n' or \c '\r\n')
 *
 * @tparam SerialPort: The serial port implementation. Defaults to asio::serial_port. Any
 *         \c SerialPort implementation must provide:
 *            * A constructor which takes an \c asio::io_service.
 *            * A `void open(const std::string& device, asio::error_code& ec)`
 *            * A `bool is_open()` function
 *            * A `bool set_option(asio::serial_port_base::baud_rate baudrate, asio::error_code& ec)`
 *            * A `void close(asio::error_code& ec)`
 *            * A `std::size_t read_some(const asio::mutable_buffers_1& buffers, asio::error_code& ec)`
 */
template <class SerialPort = asio::serial_port>
class SerialInterface
{
public:

    /**
     * Constructor.
     */
    SerialInterface() noexcept
        : io_service_()
        , serial_(std::make_unique<SerialPort>(io_service_))
    {
    }

    /**
     * Destructor.
     */
    virtual ~SerialInterface() noexcept
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
    virtual bool open(
            std::string port,
            uint64_t baudrate) noexcept
    {
        if (!is_open())
        {
            asio::error_code ec;
            serial_->open(port, ec);
            if (!ec)
            {
                serial_->set_option(asio::serial_port_base::baud_rate(baudrate), ec);
                if (!ec)
                {
                    return true;
                }
            }
            std::cout << "[ERROR] Cannot open serial port '" << port << "'. Error: " << ec.message() << std::endl;
        }
        return false;
    }

    virtual bool is_open() noexcept
    {
        return serial_->is_open();
    }

    /**
     * Close the serial port
     *
     * \pre The port was open
     * \return true if the port was open upon the call to \c close() and closed afterwards, or if
     *         the port was already closed upon the call to \c close(). false if the port was open
     *         and could not be closed.
     */
    virtual bool close() noexcept
    {
        if (is_open())
        {
            asio::error_code ec;
            serial_->close(ec);
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
     * \param[out] result A string to store the read line. If the serial connection is not opened,
     *             then @param result will not be modified. If it is open, then it will be cleared
     *             first with \c std::string::clear(). In case of error while reading on an opened
     *             port, @param result will either be empty (when error reading the first
     *             character), or have just the characters read (but not the original content).
     * \return true if a line was read; false if the port was closed or there was an error while
     *         reading.
     */
    virtual bool read_line(
            std::string& result) noexcept
    {
        if (!is_open())
        {
            return false;
        }

        char c;
        result.clear();
        asio::error_code ec;
        while (true)
        {
            read_char(c, ec);
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
                switch (ec.value())
                {
                    case asio::error::operation_aborted:
                    {
                        std::cout << "[INFO] Read operation aborted" << std::endl;
                        break;
                    }
                    default:
                    {
                        std::cout << "[ERROR] Something happened while reading: " << ec.message()
                                  << std::endl;
                        close();
                        break;
                    }
                }
                return false;
            }
        }
    }

protected:

    //! Asio's I/O service. It is used to construct the \c SerialPort
    asio::io_service io_service_;

    //! Unique pointer to the \c SerialPort implementation (defaults to \c asio::serial_port)
    std::unique_ptr<SerialPort> serial_;

    /**
     * Read a character from the serial port.
     *
     * This function blocks until either:
     *    1. A character is read
     *    2. The serial port is closed
     *    3. An internal error occurs on asio::async_read_some
     *
     * @param[out] c Reference to the read char
     * @param ec The error code returned by asio::async_read_some
     * @return The number of read characters, i.e. 1 when something is read, 0 when it's not
     */
    virtual std::size_t read_char(
            char& c,
            asio::error_code& ec) noexcept
    {
        std::size_t ret = 1;
        std::function<void (const asio::error_code&, std::size_t)> on_char_read = [&](
            const asio::error_code& error_code,
            std::size_t bytes_transferred)
                {
                    if (error_code)
                    {
                        ec = error_code;
                        ret = 0;
                    }
                };

        c = '\0';
        // Prepare io_service, give it some work, and run it
        io_service_.reset();
        serial_->async_read_some(asio::buffer(&c, 1), on_char_read);
        io_service_.run();  // This will block until a new char is ready
        return ret;
    }

};

} // namespace eduponz
} // namespace opennmea

#endif  /* _OPENNMEA_SERIALINTERFACE_H */
