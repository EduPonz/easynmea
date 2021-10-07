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

/**
 * @file snippets.cpp
 */

#include <iostream>

#include <easynmea/EasyNmea.hpp>

namespace eduponz {
namespace easynmea {
namespace docs_snippets {

void usage_snippets()
{
    {
        //USAGE_BASIC
        using namespace eduponz::easynmea;
        // Create an EasyNmea object
        EasyNmea easynmea;
        // Open the serial port
        if (easynmea.open("/dev/ttyACM0", 9600) == ReturnCode::RETURN_CODE_OK)
        {
            // Create a mask to only wait on data from specific NMEA 0183 sentences
            NMEA0183DataKindMask data_kind_mask = NMEA0183DataKind::GPGGA;
            // This call will block until some data of any of the kinds specified in the mask is
            // available.
            while (easynmea.wait_for_data(data_kind_mask) == ReturnCode::RETURN_CODE_OK)
            {
                // Take all the available data samples of type GPGGA
                GPGGAData gpgga_data;
                while (easynmea.take_next(gpgga_data) == ReturnCode::RETURN_CODE_OK)
                {
                    // Do something with the GNSS data
                    std::cout << "GNSS position: (" << gpgga_data.latitude << "; "
                              << gpgga_data.longitude << ")" << std::endl;
                }
            }
        }
        // Close the serial connection
        easynmea.close();
        //!--
    }
}

} // namespace docs_snippets
} // namespace easynmea
} // namespace eduponz

/**
 * Create an executable from the file in case future tests are added
 */
int main(
        int argc,
        char* argv[])
{
    return 0;
}