[![Documentation Status](https://readthedocs.org/projects/gnss-interface-docs/badge/?version=latest)](https://gnss-interface-docs.readthedocs.io/en/latest/?badge=latest)

# GNSS Interface {#index}

**gnss_interface** is a `C++` library to get **GNSS** information from GNSS modules which communicate with **NMEA 0183** over UART.
It has been originally developed targeting Quectel GNSS L86 modules, however, it can retrieve GNSS data from any GNSS device sending **NMEA 0183** sentences over UART.

* [Features](#features)
* [Getting started](#getting-started)
    * [Prerequisites](#prerequisites)
    * [Build and install](#build-and-install)
    * [Build documentation](#build-documentation)
    * [Run examples](#run-examples)
* [Usage](#usage)
* [Authors](#authors)
* [License](#license)
* [To-Do](#to-do)

For more detailed information, please visit the [GNSS Interface documentation](https://gnss-interface-docs.readthedocs.io/).

## Features

**gnss_interface** provides the `GnssInterface` class, which uses **NMEA 0183** `$GPGGA` sentences to extract **GNSS** information from the modules, providing a simple and easy-to-use API.

## Getting started

**gnss_interface** has been developed and tested in Ubuntu 20.04 Operating System, although it is expected to support Windows 10 and MacOS thanks to [Asio](https://github.com/chriskohlhoff/asio), the cross-platform library used to interact with the serial ports.

### Prerequisites

**gnss_interface** relies on [Asio](https://github.com/chriskohlhoff/asio) for establishing a serial connection with the GNSS module.
On Ubuntu platforms, this can be installed with:

```bash
sudo apt install libasio-dev
```

### Build and install

To build the library, run:

```bash
cd
git clone https://github.com/EduPonz/gnss_interface.git
mkdir build
cd build
cmake ..
cmake --build .
```

Once built, the library can be installed in a user specified directory with:

```bash
cd ~/gnss_interface/build
cmake .. -DCMAKE_INSTALL_PREFIX=<user-specified-dir>
cmake --build . --target install
```

To install the library system-wide, just omit the `CMAKE_INSTALL_PREFIX`:

```bash
cd ~/gnss_interface/build
cmake ..
cmake --build . --target install
```

### Build documentation

It is possible to generate the library's documentation passing CMake option `-DBUILD_DOCUMENTATION=0N` (defaults to `OFF`) on the configuration step:

```bash
cd ~/gnss_interface/build
cmake .. -DCMAKE_INSTALL_PREFIX=<user-specified-dir> -DBUILD_DOCUMENTATION=ON
cmake --build . --target install
```

### Run examples

It is also possible to build and install the library's examples passing CMake option `-DBUILD_EXAMPLES=ON` (defaults to `OFF`) on the configuration step:

```bash
cd ~/gnss_interface/build
cmake .. -DCMAKE_INSTALL_PREFIX=<user-specified-dir> -DBUILD_EXAMPLES=ON
cmake --build . --target install
```

Then, they can be run with:

```bash
cd <user-specified-dir>/examples/bin
./gpgga_example
```

An output example from `gpgga_example` would be:

```
Serial port '/dev/ttyACM0' opened. Baudrate: 9600
Please press CTRL-C to stop the example

************** NEW GPGGA SAMPLE **************
Elapsed time ---------> 3468
------------------------------------------
GPGGA Data - GNSS Position Fix
==============================
Message --------------> $GPGGA,072706.000,5703.1740,N,00954.9459,E,1,8,1.28,-21.2,M,42.5,M,,*4E
Timestamp ------------> 72706
Latitude -------------> 57.0529º N
Longitude ------------> 9.91576º E
Fix ------------------>
Number of satellites -> 8
Horizontal precision -> 1.28
Altitude -------------> -21.2
```

The some example's parameters can be configured using command line options.
Run `./gpgga_example --help`

```
------------------------------
GNSS Interface - GPGGA Example
------------------------------
Usage: ./gpgga_example [OPTIONS]
    -h/--help:                Print this help and exit
    -b/--baudrate [bauds]:    The connection baud rate in bauds [Defaults: 9600]
    -p/--serial_port [port]:  The serial port to use [Defaults: 'dev/ttyACM0']
Example: ./gpgga_example -p /dev/ttyUSB0 -b 9600
```

## Usage

The **GNSS** information can be retrieved in the following manner (see [gpgga_example.cpp](examples/gpgga_example.cpp)):

```c++
using namespace eduponz::gnss_interface;
// Create an interface object
GnssInterface gnss;
// Open the serial port
if (gnss.open("/dev/ttyACM0", 9600) == ReturnCode::RETURN_CODE_OK)
{
    // Create a mask to only wait on data from specific NMEA 0183 sentences
    NMEA0183DataKindMask data_kind_mask = NMEA0183DataKind::GPGGA;
    // This call will block until some data of any of the kinds specified in the mask is available.
    while (gnss.wait_for_data(data_kind_mask) == ReturnCode::RETURN_CODE_OK)
    {
        // Take all the available data samples of type GPGGA
        GPGGAData gpgga_data;
        while (gnss.take_next(gpgga_data) == ReturnCode::RETURN_CODE_OK)
        {
            // Do something with the GNSS data
            std::cout << "GNSS position: (" << gpgga_data.latitude << "; " << gpgga_data.longitude << ")" << std::endl;
        }
    }
}
// Close the serial connection
gnss.close();
```

## Authors

**gnss_interface** has been developed by **Eduardo Ponz**.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## To-Do

- Add robustness to `SerialInterface`.
- Process other **NMEA 0183** sentences. See [gnss_l86_output.txt](gnss_l86_output.txt) for an example of the messages coming from the module.
- Send commands to GNSS modules using [**PMTK**](https://www.quectel.com/UploadImage/Downlad/Quectel_GNSS_SDK_Commands_Manual_V1.2.pdf) protocol.
