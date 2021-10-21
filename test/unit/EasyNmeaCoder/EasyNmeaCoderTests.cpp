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

#include <cmath>
#include <memory>
#include <thread>

#include <gtest/gtest.h>

#include <easynmea/types.hpp>
#include <EasyNmeaCoder.hpp>

using namespace eduponz::easynmea;

TEST(EasyNmeaCoderTests, decodeGPGGAValidNE)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*4a";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));

    ASSERT_EQ(data->kind, NMEA0183DataKind::GPGGA);
    ASSERT_FLOAT_EQ(data->timestamp, 72705);
    ASSERT_FLOAT_EQ(data->latitude, 57.0529);
    ASSERT_FLOAT_EQ(data->longitude, 9.9157648);
    ASSERT_EQ(data->fix, 1);
    ASSERT_EQ(data->satellites_on_view, 7);
    ASSERT_FLOAT_EQ(data->horizontal_precision, 1.97);
    ASSERT_FLOAT_EQ(data->altitude, -21.2);
    ASSERT_FLOAT_EQ(data->height_of_geoid, 42.5);
    ASSERT_FLOAT_EQ(data->dgps_last_update, 2.2);
    ASSERT_EQ(data->dgps_reference_station_id, 7854);
}

TEST(EasyNmeaCoderTests, decodeGPGGAValidNW)
{
    std::string sentence = "$GPGGA,072706.000,5703.1740,N,00954.9459,W,1,8,1.28,-21.2,M,42.5,M,2.2,7854,*50";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));

    ASSERT_EQ(data->kind, NMEA0183DataKind::GPGGA);
    ASSERT_FLOAT_EQ(data->timestamp, 72706);
    ASSERT_FLOAT_EQ(data->latitude, 57.0529);
    ASSERT_FLOAT_EQ(data->longitude, -9.9157648);
    ASSERT_EQ(data->fix, 1);
    ASSERT_EQ(data->satellites_on_view, 8);
    ASSERT_FLOAT_EQ(data->horizontal_precision, 1.28);
    ASSERT_FLOAT_EQ(data->altitude, -21.2);
    ASSERT_FLOAT_EQ(data->height_of_geoid, 42.5);
    ASSERT_FLOAT_EQ(data->dgps_last_update, 2.2);
    ASSERT_EQ(data->dgps_reference_station_id, 7854);
}

TEST(EasyNmeaCoderTests, decodeGPGGAValidSE)
{
    std::string sentence = "$GPGGA,072707.000,5703.1740,S,00954.9459,E,1,8,1.27,-21.2,M,42.5,M,2.2,7854,*51";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));

    ASSERT_EQ(data->kind, NMEA0183DataKind::GPGGA);
    ASSERT_FLOAT_EQ(data->timestamp, 72707);
    ASSERT_FLOAT_EQ(data->latitude, -57.0529);
    ASSERT_FLOAT_EQ(data->longitude, 9.9157648);
    ASSERT_EQ(data->fix, 1);
    ASSERT_EQ(data->satellites_on_view, 8);
    ASSERT_FLOAT_EQ(data->horizontal_precision, 1.27);
    ASSERT_FLOAT_EQ(data->altitude, -21.2);
    ASSERT_FLOAT_EQ(data->height_of_geoid, 42.5);
    ASSERT_FLOAT_EQ(data->dgps_last_update, 2.2);
    ASSERT_EQ(data->dgps_reference_station_id, 7854);
}

TEST(EasyNmeaCoderTests, decodeGPGGAValidSW)
{
    std::string sentence = "$GPGGA,072708.000,5703.1736,S,00954.9457,W,1,8,1.28,-21.6,M,42.5,M,2.2,7854,*48";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));

    ASSERT_EQ(data->kind, NMEA0183DataKind::GPGGA);
    ASSERT_FLOAT_EQ(data->timestamp, 72708);
    ASSERT_FLOAT_EQ(data->latitude, -57.0529);
    ASSERT_FLOAT_EQ(data->longitude, -9.9157648);
    ASSERT_EQ(data->fix, 1);
    ASSERT_EQ(data->satellites_on_view, 8);
    ASSERT_FLOAT_EQ(data->horizontal_precision, 1.28);
    ASSERT_FLOAT_EQ(data->altitude, -21.6);
    ASSERT_FLOAT_EQ(data->height_of_geoid, 42.5);
    ASSERT_FLOAT_EQ(data->dgps_last_update, 2.2);
    ASSERT_EQ(data->dgps_reference_station_id, 7854);
}

TEST(EasyNmeaCoderTests, decodeGPGGAValidNoAgeOfDiffGPS)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,,7854,*64";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));

    ASSERT_EQ(data->kind, NMEA0183DataKind::GPGGA);
    ASSERT_FLOAT_EQ(data->timestamp, 72705);
    ASSERT_FLOAT_EQ(data->latitude, 57.0529);
    ASSERT_FLOAT_EQ(data->longitude, 9.9157648);
    ASSERT_EQ(data->fix, 1);
    ASSERT_EQ(data->satellites_on_view, 7);
    ASSERT_FLOAT_EQ(data->horizontal_precision, 1.97);
    ASSERT_FLOAT_EQ(data->altitude, -21.2);
    ASSERT_FLOAT_EQ(data->height_of_geoid, 42.5);
    ASSERT_FLOAT_EQ(data->dgps_last_update, -1);
    ASSERT_EQ(data->dgps_reference_station_id, 7854);
}

TEST(EasyNmeaCoderTests, decodeGPGGAValidEmptyAgeOfDiffGPSNoDiffRefStation)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,,*46";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));

    ASSERT_EQ(data->kind, NMEA0183DataKind::GPGGA);
    ASSERT_FLOAT_EQ(data->timestamp, 72705);
    ASSERT_FLOAT_EQ(data->latitude, 57.0529);
    ASSERT_FLOAT_EQ(data->longitude, 9.9157648);
    ASSERT_EQ(data->fix, 1);
    ASSERT_EQ(data->satellites_on_view, 7);
    ASSERT_FLOAT_EQ(data->horizontal_precision, 1.97);
    ASSERT_FLOAT_EQ(data->altitude, -21.2);
    ASSERT_FLOAT_EQ(data->height_of_geoid, 42.5);
    ASSERT_FLOAT_EQ(data->dgps_last_update, -1);
    ASSERT_EQ(data->dgps_reference_station_id, 0);
}

TEST(EasyNmeaCoderTests, decodeGPGGAValidNoDiffRefStation)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,*68";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));

    ASSERT_EQ(data->kind, NMEA0183DataKind::GPGGA);
    ASSERT_FLOAT_EQ(data->timestamp, 72705);
    ASSERT_FLOAT_EQ(data->latitude, 57.0529);
    ASSERT_FLOAT_EQ(data->longitude, 9.9157648);
    ASSERT_EQ(data->fix, 1);
    ASSERT_EQ(data->satellites_on_view, 7);
    ASSERT_FLOAT_EQ(data->horizontal_precision, 1.97);
    ASSERT_FLOAT_EQ(data->altitude, -21.2);
    ASSERT_FLOAT_EQ(data->height_of_geoid, 42.5);
    ASSERT_FLOAT_EQ(data->dgps_last_update, 2.2);
    ASSERT_EQ(data->dgps_reference_station_id, 0);
}

TEST(EasyNmeaCoderTests, decodeGPGGAValidNoOptionals)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,*6a";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));

    ASSERT_EQ(data->kind, NMEA0183DataKind::GPGGA);
    ASSERT_FLOAT_EQ(data->timestamp, 72705);
    ASSERT_FLOAT_EQ(data->latitude, 57.0529);
    ASSERT_FLOAT_EQ(data->longitude, 9.9157648);
    ASSERT_EQ(data->fix, 1);
    ASSERT_EQ(data->satellites_on_view, 7);
    ASSERT_FLOAT_EQ(data->horizontal_precision, 1.97);
    ASSERT_FLOAT_EQ(data->altitude, -21.2);
    ASSERT_FLOAT_EQ(data->height_of_geoid, 42.5);
    ASSERT_FLOAT_EQ(data->dgps_last_update, -1);
    ASSERT_EQ(data->dgps_reference_station_id, 0);
}

TEST(EasyNmeaCoderTests, decodeInvalidSentenceID)
{
    std::string sentence = "$ABCDE,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*5d";
    std::shared_ptr<NMEA0183Data> data = EasyNmeaCoder::decode(sentence);
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGAInvalidTime)
{
    std::string sentence = "$GPGGA,123,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*63";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGAInvalidLatitudeLength)
{
    std::string sentence = "$GPGGA,072705.000,123.123,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*49";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGAInvalidLatitudeDegrees)
{
    std::string sentence = "$GPGGA,072705.000,9903.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*48";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);

}

TEST(EasyNmeaCoderTests, decodeGPGGAInvalidLatitudeMinutes)
{
    std::string sentence = "$GPGGA,072705.000,5761.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*4e";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGAInvalidLongitudeLength)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,123.123,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*73";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGAInvalidLongitudeDegrees)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,18154.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*4b";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGAInvalidLongitudeMinutes)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00961.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*4c";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGAInvalidAltitudeUnits)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,A,42.5,M,2.2,7854,*46";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGAInvalidHeightUnits)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,A,2.2,7854,*46";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGAInvalidChecksum)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*22";
    std::shared_ptr<NMEA0183Data> data = EasyNmeaCoder::decode(sentence);
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGANoTime)
{
    std::string sentence = "$GPGGA,,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*53";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGANoLatitude)
{
    std::string sentence = "$GPGGA,072705.000,,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*67";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGANoLongitude)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*5d";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGANoFix)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,,7,1.97,-21.2,M,42.5,M,2.2,7854,*7b";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGANoNumberOfSatellites)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,,1.97,-21.2,M,42.5,M,2.2,7854,*7d";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGANoHDOP)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,,-21.2,M,42.5,M,2.2,7854,*5b";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGANoAltitude)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,,M,42.5,M,2.2,7854,*78";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGANoHeight)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,,M,2.2,7854,*57";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeGPGGANoChecksum)
{
    std::string sentence = "$GPGGA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854";
    std::shared_ptr<GPGGAData> data = std::static_pointer_cast<GPGGAData>(EasyNmeaCoder::decode(sentence));
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeUnsupportedSentence)
{
    std::vector<std::string> sentences = {
        "$GPBOD,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*42",
        "$GPBWC,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*5d",
        "$GPGLL,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*4c",
        "$GPGSA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*5e",
        "$GPGSV,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*49",
        "$GPHDT,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*53",
        "$GPR00,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*59",
        "$GPRMA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*55",
        "$GPRMB,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*56",
        "$GPRMC,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*57",
        "$GPRTE,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*48",
        "$GPTRF,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*4b",
        "$GPSTN,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*42",
        "$GPVBW,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*48",
        "$GPVTG,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*4e",
        "$GPWPL,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*40",
        "$GPXTE,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*42",
        "$GPZDA,072705.000,5703.1740,N,00954.9459,E,1,7,1.97,-21.2,M,42.5,M,2.2,7854,*54"
    };

    for (std::string sentence : sentences)
    {
        std::shared_ptr<NMEA0183Data> data = EasyNmeaCoder::decode(sentence);
        ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
    }
}

TEST(EasyNmeaCoderTests, decodeEmptySentence)
{
    std::string sentence = "";
    std::shared_ptr<NMEA0183Data> data = EasyNmeaCoder::decode(sentence);
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeOnlyChecksumSentence)
{
    std::string sentence = "*0";
    std::shared_ptr<NMEA0183Data> data = EasyNmeaCoder::decode(sentence);
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

TEST(EasyNmeaCoderTests, decodeOnlyAstheriscSentence)
{
    std::string sentence = "*";
    std::shared_ptr<NMEA0183Data> data = EasyNmeaCoder::decode(sentence);
    ASSERT_EQ(data->kind, NMEA0183DataKind::INVALID);
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
