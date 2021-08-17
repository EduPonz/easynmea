# GNSS L86 Interface

**gnss_l86_interface** is a `C++` library to get **GNSS** information from Quectel GNSS L86 modules using UART communication.

* [Features](#features)
* [Getting started](#getting-started)
    * [Prerequisites](#prerequisites)
    * [Build and install](#build-and-install)
    * [Run examples](#run-examples)
* [Usage](#usage)
* [Authors](#authors)
* [License](#license)
## To-Do

## Features


**gnss_l86_interface** provides the `GnssInterface` class, which uses **NMEA 0183** `$GPGGA` messages to extract **GNSS** information from Quectel GNSS L86 modules.
The information is stored in the following `struct`:

```c++
struct position
{
    std::string message;        // The received message
    float timestamp;            // UTC time hhmmss.milliseconds
    float latitude;             // Always referred to North
    float longitude;            // Always referred to East
    int fix;                    // 0 -> no fix, 1 -> fix, 2 -> dif fix
    int number_of_satelites;    // Satellites on view
    float horizontal_precision; // In meters
    float altitude;             // Over sea level
};
```

## Getting started

**gnss_l86_interface** has been developed and tested in November 2018 for using a Raspberry Pi 3 model B with **Raspbian GNU/Linux 9 (stretch)** as Operating System.
Other platforms, operating systems or distributions have not been tested and may require source code modification.

### Prerequisites

**gnss_l86_interface** relies on [wiringPi](http://wiringpi.com/) for establishing a serial connection with the GNSS module.

### Build and install

To build the library, run:

```bash
cd
git clone https://github.com/EduPonz/gnss_l86_lib.git gnss_l86_interface
mkdir build
cd build
cmake ..
cmake --build .
```

Once built, the library can be installed in a user specify directory with:

```bash
cd ~/gnss_l86_interface/build
cmake .. -DCMAKE_INSTALL_PREFIX=<user-specified-dir>
cmake --build . --target install
```

To install the library system-wide, just omit the `CMAKE_INSTALL_PREFIX`:

```bash
cd ~/gnss_l86_interface/build
cmake ..
cmake --build . --target install
```

### Run examples

It is also possible to build and install the library's examples passing CMake option `-DBUILD_EXAMPLES=ON` (defaults to `OFF`) on the configuration step:

```bash
cd ~/gnss_l86_interface/build
cmake .. -DCMAKE_INSTALL_PREFIX=<user-specified-dir> -DBUILD_EXAMPLES=ON
cmake --build . --target install
```

Then, they can be run with:

```bash
cd <user-specified-dir>examples/bin
./gnss_l86_interface_example
```

An output example from `gnss_l86_interface_example` would be:

```
Connected to port: 3

********************************** NEW POSITION **********************************
Ellapsed time --------> 1002
Number of lines read -> 1
----------------------------------------------------------------------------------
POSITION
========================
Message --------------> $GPGGA,090115.000,5703.1744,N,00954.9365,E,1,11,1.14,24.4,M,42.5,M,,*53
Timestamp ------------> 90115.000
Latitude -------------> 57.0529
Longitude ------------> 9.91561
Fix ------------------> 1
Number of satellites -> 11
Horizontal Precision -> 1.14
Altitude -------------> 24.4
```

## Usage

The **GNSS** information can be retrieved in the following manner (see [gnss_l86_example.cpp](examples/gnss_l86_example.cpp)):

```c++
GnssInterface gnss;

if (gnss.open_connection("/dev/serial0", 9600))
{
    int num_lines = gnss.read_lines();
    position last_position = gnss.get_position();
}

gnss.close_connection();
```

## Authors

**gnss_l86_interface** has been developed by **Eduardo Ponz**.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## To-Do

- Process other **NMEA 0183** message types. See [gnss_l86_output.txt](gnss_l86_output.txt) for an example of the messages coming from the module.
- Send commands to the GNSS module using [**PMTK**](https://www.quectel.com/UploadImage/Downlad/Quectel_GNSS_SDK_Commands_Manual_V1.2.pdf) protocol.
