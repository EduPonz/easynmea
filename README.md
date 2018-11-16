Quectel GNSS L86 - C++ Library
==============================
**gnss_l86_lib** is a `c++` library to get **GNSS** information from Quectel GNSS L86 modules using UART communication.

Features
--------
**gnss_l86_lib** provides the `GnssInterface` class, which uses **NMEA 0183** `$GPGGA` messages to extract **GNSS** information from Quectel GNSS L86 modules. The information is stored in the following `struct`:

```c++
struct position {
    std::string message;        // The received message
    float timestamp;            // UTC time hhmmss.milliseconds
    float latitude;             // Always referred to North
    float longitude;            // Always referred to East
    int fix;                    // 0 -> no fix, 1 -> fix, 2 -> dif fix
    int number_of_satelites;    // Satelites on view
    float horizontal_precision; // In meters;
    float altitude;             // Over sea level
};
```
Getting Started
---------------
**gnss_l86_lib** has been developed and tested in November 2018 for using a Raspberry Pi 3 model B with **Raspbian GNU/Linux 9 (stretch)** as Operating System. Other platforms, operating systems or distributions have not been tested and may requiere source code modification.

### Prerequisites
**gnss_l86_lib** relies on [wiringPi](http://wiringpi.com/) for establishing a serial connection with the GNSS module.

Build Example
-------------
To build and try the [gnss_l86_example](gnss_l86_example.cpp) run:
``` 
$ make
$ sudo ./gnss_l86_example
```

Usage
-----
The **GNSS** information can be retrieved in the following manner (see [gnss_l86_example.cpp](gnss_l86_example.cpp)):

```c++
GnssInterface gnss;

if (gnss.open_connection("/dev/serial0", 9600))
{
    int num_lines = gnss.read_lines();
    position last_position = gnss.get_position();
}

gnss.close_connection();
```

### Output example (from [gnss_l86_example.cpp](gnss_l86_example.cpp))
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
Number of satelites --> 11
Horizontal Precision -> 1.14
Altitude -------------> 24.4
```

Authors
-------
**gnss_l86_lib** has been developed by **Eduardo Ponz**.

License
-------
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

To-Do
-----
- Process other **NMEA 0183** message types. See [gnss_l86_output.txt](gnss_l86_output.txt) for an example of the messages comming from the module.
- Send commands to the GNSS module using [**PMTK**](https://www.quectel.com/UploadImage/Downlad/Quectel_GNSS_SDK_Commands_Manual_V1.2.pdf) protocol.
