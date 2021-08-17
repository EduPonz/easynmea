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

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <wiringPi.h>
#include <wiringSerial.h>

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

class GnssInterface
{
    private:
        const std::string POSITION_START_ = "$GPGGA";
        std::string read_line_;
        int port_;
        position position_;

        std::vector<std::string> break_string_(std::string str, char separator);
        float parse_to_degrees_(std::string str);
        bool parse_raw_line_(std::string line);
        bool populate_position_(std::string position_line);
        std::vector<std::string> read_raw_lines_();
    public:
        GnssInterface();
        ~GnssInterface();
        bool close_connection();
        position get_position();
        int get_port();
        bool open_connection(const char* serial_port, long baud_rate);
        int read_lines();
};
