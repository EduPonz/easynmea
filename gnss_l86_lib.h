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
