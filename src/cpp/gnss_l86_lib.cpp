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

#include "gnss_l86_lib.h"

// ******************************** CONSTRUCTORS-DESTRUCTORS *******************************
GnssInterface::GnssInterface()
{
    read_line_ = "";
    port_ = 0;
    position_.timestamp = 0;
    position_.latitude = 0;
    position_.longitude = 0;
    position_.fix = 0;
    position_.number_of_satelites = 0;
    position_.horizontal_precision = 0;
    position_.altitude = 0;
}

GnssInterface::~GnssInterface() { }

// **************************************** PRIVATE ****************************************
std::vector<std::string> GnssInterface::break_string_(std::string str, char separator)
{
    std::string result = "";
    std::stringstream ss(str);
    std::string substr;
    std::vector<std::string> content;

    while (ss.good())
    {
        getline(ss, substr, separator);
        if (substr != "")
            content.push_back(substr);
    }

    return content;
}

float GnssInterface::parse_to_degrees_(std::string str)
{
    std::vector<std::string> content = break_string_(str, '.');

    std::string minutes = content[0].substr(content[0].size() - 2);
    minutes += "." + content[1];

    std::string::size_type idx;
    float minutes_float = std::stof(minutes, &idx);
    minutes_float = minutes_float / 60;

    content[0].erase(content[0].end() - 2, content[0].end());
    idx = 0;
    float degrees = std::stof(content[0], &idx) + minutes_float;

    return degrees;
}

bool GnssInterface::parse_raw_line_(std::string line)
{
    if (strncmp(line.c_str(), POSITION_START_.c_str(), POSITION_START_.size()) == 0)
        return populate_position_(line);
    else
        return false;
}

bool GnssInterface::populate_position_(std::string position_line)
{
    std::vector<std::string> content = break_string_(position_line, ',');

    if (content.size() >= 10)
    {
        std::string::size_type idx;

        position_.message = position_line;
        position_.timestamp = std::stof(content[1], &idx);
        idx = 0;

        position_.latitude = parse_to_degrees_(content[2]);
        position_.longitude = parse_to_degrees_(content[4]);
        if (content[3] != "N") position_.latitude = -position_.latitude;
        if (content[5] != "E") position_.longitude = -position_.longitude;

        position_.fix = std::stoi(content[6], &idx);
        idx = 0;
        position_.number_of_satelites = std::stoi(content[7], &idx);
        idx = 0;
        position_.horizontal_precision = std::stof(content[8], &idx);
        idx = 0;
        position_.altitude = std::stof(content[9], &idx);

        return true;
    }
    else
    {
        position_.fix = 0;
        return false;
    }
}

std::vector<std::string> GnssInterface::read_raw_lines_()
{
    char received;
    std::vector<std::string> lines;
    while (serialDataAvail(port_))
    {
        received = serialGetchar(port_);
        if (received != '\n')
            read_line_ += received;
        else
        {
            lines.push_back(read_line_);
            read_line_ = "";
        }
    }

    return lines;
}

// **************************************** PUBLIC *****************************************
bool GnssInterface::close_connection()
{
    serialClose(port_);
    return true;
}

int GnssInterface::get_port()
{
    return port_;
}

position GnssInterface::get_position()
{
    return position_;
}

bool GnssInterface::open_connection(const char* serial_port, long baud_rate)
{
    port_ = serialOpen(serial_port, baud_rate);

    if (port_ >= 0)
    {
        read_raw_lines_();
        return true;
    }
    else return false;
}

int GnssInterface::read_lines()
{
    std::vector<std::string> raw_lines = read_raw_lines_();
    int num_lines = 0;

    for (int i = 0; i < int(raw_lines.size()); i++)
        if (parse_raw_line_(raw_lines[i])) num_lines++;

    return num_lines;
}
