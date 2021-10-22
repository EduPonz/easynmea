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
 * @file EasyNmeaCoder.hpp
 */

#ifndef _EASYNMEA_DECODER_HPP_
#define _EASYNMEA_DECODER_HPP_

#include <iostream>
#include <memory>
#include <regex>
#include <sstream>
#include <string>

#include <easynmea/data.hpp>
#include <easynmea/types.hpp>

namespace eduponz {
namespace easynmea {
namespace nmea0183 {

const char* GPGGA_ID = "$GPGGA";

const char* NMEA0183_BASIC_REGEX =
        "\\$[A-Z0-9]+"        // Sentence ID
        "[0-9a-zA-Z,.\\-]*"   // NMEA 0183 data in any form
        "\\*[0-9a-fA-F]{2}";  // Checksum

const char* GPGGA_REGEX =
        "\\$GPGGA,"                                                                  // GPGGA sentence ID
        "[0-9]{6}(\\.[0-9]+)?,"                                                      // Timestamp in hhmmss.ss
        "((([0-8][0-9])(([0-5][0-9])|(60))\\.[0-9]+)|(90\\.[0]+)),"                  // Latitude as DDMM.mm [0; 90]
        "[NS],"                                                                      // Beraing of latitude
        "((((0[0-9]{2})|(1[0-7][0-9]))(([0-5][0-9])|(60))\\.[0-9]+)|(180\\.[0]+)),"  // Longitude as DDDMM.mm [0; 180]
        "[EW],"                                                                      // Bearing of longitude
        "[0-2],"                                                                     // GPS fix
        "[0-9]+,"                                                                    // Number of satellites
        "[0-9]+\\.[0-9]+,"                                                           // HDOP
        "[\\-]?[0-9]+\\.[0-9]+,"                                                     // Altitude
        "[M],"                                                                       // Altitude units
        "[\\-]?[0-9]+\\.[0-9]+,"                                                     // Height of geoid above WGS84
        "[M],"                                                                       // Height units
        "("                                                                          // Start of optional fields
        "([0-9]+\\.[0-9]+)?,"                                                        // Time since last DGPS update
        "([0-9]+,)?"                                                                 // DGPS reference station id
        ")?"                                                                         // End of optional fields
        "\\*[0-9a-fA-F]{2}";                                                         // Checksum

} // namespace nmea0183

/**
 * @class EasyNmeaCoder
 *
 * This class provides static APIs to decode NMEA 0183 sentences.
 */
class EasyNmeaCoder
{
public:

    /**
     * \brief Decode a NMEA 0183 sentence
     *
     * @param sentence The sentence to be decoded
     * @return A shared pointer to a NMEA0183Data. The \c kind field can be used in combination with
     *         \c std::static_pointer_cast to cast the pointer to the appropriate type. The function
     *         transfers the shared pointer ownership to the caller, thus the returned pointer is
     *         the only reference to the data object.
     */
    static std::shared_ptr<NMEA0183Data> decode(
            const std::string& sentence) noexcept
    {
        // Check that the sentence resembles a NMEA 0183 sentence
        if (!std::regex_match(sentence, std::regex(nmea0183::NMEA0183_BASIC_REGEX)))
        {
            std::cout << "[WARNING] Sentence '" << sentence << "' is NOT a valid NMEA 0183 sentence" << std::endl;
            return std::move(std::make_shared<NMEA0183Data>());
        }

        // Check that the checksum is correct
        if (!validate_checksum_(sentence))
        {
            std::cout << "[WARNING] Sentence: '" << sentence << "' has an incorrect checksum" << std::endl;
            return std::move(std::make_shared<NMEA0183Data>());
        }

        // Decode according to sentence identifier
        switch (data_kind_(sentence))
        {
            case NMEA0183DataKind::GPGGA:
            {
                return std::move(decode_gpgga_(sentence));
            }
            default:
            {
                return std::move(std::make_shared<NMEA0183Data>());
            }
        }
    }

protected:

    /**
     * \brief Validate a NMEA 0183 sentence checksum
     *
     * The NMEA 0183 checksum is a hexadecimal number calculated as a result of XORing all the
     * characters in the sentence from the starting '$' until the checksum mark '*' (none of them
     * included).
     *
     * \pre \c validate_checksum_ assumes that the sentence is a valid NMEA sentence, meaning that
     * it starts with a '$', and ends with a '*<hexadecimal number>'.
     *
     * @param sentence The sentence which checksum is to be checked
     * @return \c true if the sentence contains a correct checksum; \c false otherwise.
     */
    static bool validate_checksum_(
            const std::string& sentence) noexcept
    {
        /* Separate sentence from checksum */
        std::string sentence_no_check;
        std::string checksum_str;
        std::stringstream sentence_ss(sentence);
        std::getline(sentence_ss, sentence_no_check, '*');
        std::getline(sentence_ss, checksum_str, '*');
        sentence_no_check = sentence_no_check.substr(1); // Remove first char from sentence ($)

        /* Convert hex string to int */
        int checksum = 0;
        std::stringstream checksum_ss;
        checksum_ss << std::hex << checksum_str;
        checksum_ss >> checksum;

        /* Calculate checksum from sentence */
        int check = 0;
        for (char character : sentence_no_check)
        {
            check ^= character;
        }

        /* Validate input checksum */
        return check == checksum;
    }

    /**
     * \brief Return the NMEA 0183 sentence kind of a sentence using the sentence identifier.
     *
     * @param sentence The sentence from which the kind is returned.
     * @return A \c NMEA0183DataKind representing the sentence kind.
     */
    static NMEA0183DataKind data_kind_(
            const std::string& sentence) noexcept
    {
        std::string sentence_id;
        std::stringstream sentence_ss(sentence);
        std::getline(sentence_ss, sentence_id, ',');
        if (sentence_id == nmea0183::GPGGA_ID)
        {
            return NMEA0183DataKind::GPGGA;
        }
        std::cout << "[WARNING] Sentence identifier '" << sentence_id << "' is NOT supported" << std::endl;
        return NMEA0183DataKind::INVALID;
    }

    /**
     * \brief Translate a NMEA 0183 GPGGA sentence into a \c GPGGAData object
     *
     * @param sentence The sentence to be decoded.
     * @return A \c std::shared_ptr<GPGGAData> containing the GPGGA data. If for any reason the
     *         sentence is not a valid GPGGA, then the \c kind field of the returned \c GPGGAData
     *         is set to \c NMEA0183DataKind::INVALID. The function transfers the shared pointer
     *         ownership to the caller, thus the returned pointer is the only reference to the data
     *         object.
     */
    static std::shared_ptr<GPGGAData> decode_gpgga_(
            const std::string& gpgga_sentence) noexcept
    {
        std::shared_ptr<GPGGAData> data = std::make_shared<GPGGAData>();

        /* Check that the sentence is a valid GPGGA sentence */
        if (!std::regex_match(gpgga_sentence, std::regex(nmea0183::GPGGA_REGEX)))
        {
            std::cout << "[WARNING] Sentence '" << gpgga_sentence << "' is NOT a valid GPGGA sentence" << std::endl;
            data->kind = NMEA0183DataKind::INVALID;
            return std::move(data);
        }

        /* Separate each of the sentence components */
        std::vector<std::string> gpgga_data_strs = split_(gpgga_sentence);

        /*
         * Translate NMEA 0183 data to GPGGAData. At this point, the REGEX has already verified that
         * the fields have a valid format, so it is safe to avoid further checks here.
         */
        data->timestamp = std::stof(gpgga_data_strs[1]);
        data->latitude = to_degrees_(gpgga_data_strs[2]);
        data->longitude = to_degrees_(gpgga_data_strs[4]);
        // Always return latitude bearing North
        if (gpgga_data_strs[3] != "N")
        {
            data->latitude = -data->latitude;
        }
        // Always return longitude bearing East
        if (gpgga_data_strs[5] != "E")
        {
            data->longitude = -data->longitude;
        }
        data->fix = std::stoi(gpgga_data_strs[6]);
        data->satellites_on_view = std::stoi(gpgga_data_strs[7]);
        data->horizontal_precision = std::stof(gpgga_data_strs[8]);
        data->altitude = std::stof(gpgga_data_strs[9]);
        data->height_of_geoid = std::stof(gpgga_data_strs[11]);
        // Last DGPS update time is optional and can be present but empty
        if (gpgga_data_strs.size() >= 15 && gpgga_data_strs[13] != "")
        {
            data->dgps_last_update = std::stof(gpgga_data_strs[13]);
        }
        // DGPS reference station ID is optional
        if (gpgga_data_strs.size() == 16)
        {
            data->dgps_reference_station_id = std::stoi(gpgga_data_strs[14]);
        }

        /* Return the data transfering the ownership */
        return std::move(data);
    }

    /**
     * \brief Split a string in substrings according to a separator.
     *
     * @param sentence The string to be split.
     * @param separator The character used to split the sentence [Defaults: ','].
     * @return A \c std::vector<std::string> containing the substring (the separator characters are
     *         not included in the substrings).
     */
    static std::vector<std::string> split_(
            const std::string& sentence,
            char separator = ',') noexcept
    {
        std::string result = "";
        std::stringstream ss(sentence);
        std::vector<std::string> content;

        while (ss.good())
        {
            std::getline(ss, result, separator);
            content.push_back(result);
        }

        return content;
    }

    /**
     * \brief Translate a NMEA 0183 angle representation into a decimal floating point representing
     *        degrees.
     *
     * \pre The expected format of nmea0183_angle is 'D{1,3}MM.[m]+' where:
     *    1. D corresponds to degrees
     *    1. MM.[m]+ is a float representing minutes of degree
     *
     * @param nmea0183_angle The angle to be translated.
     * @return A \c float representing the angle in degrees.
     */
    static float to_degrees_(
            const std::string& nmea0183_angle) noexcept
    {
        /* Break the string into its 'D{1,3}MM' and '[m]+' components */
        std::vector<std::string> content = split_(nmea0183_angle, '.');

        /* Build a string 'MM.[m]+' to represent the minutes of degree */
        std::string minutes = content[0].substr(content[0].size() - 2) + "." + content[1];

        /* Convert the 'MM.[m]+' string into a floating point number in degrees */
        float minutes_float = std::stof(minutes) / 60;

        /* Remove the 'MM' component from 'D{1,3}MM' */
        content[0].erase(content[0].end() - 2, content[0].end());

        /* Return the degrees as the summation of 'D{1,3}' plus the 'MM.[m]+' translated into degrees */
        return std::stof(content[0]) + minutes_float;
    }

};

} // namespace eduponz
} // namespace easynmea

#endif //_EASYNMEA_DECODER_HPP_
