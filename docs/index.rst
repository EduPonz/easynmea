.. _index:

GNSS Interface Documentation
============================

.. toctree::
   :maxdepth: 3
   :hidden:

   rst/installation/installation
   rst/usage/usage
   rst/nmea_data_types/nmea_data_types
   rst/examples/examples
   rst/api_reference/api_reference

*GNSS Interface* is an open source, free-to-use cross-platform `C++` library to retrieve **Global Navigation Satellite
System** (`GNSS <https://en.wikipedia.org/wiki/Satellite_navigation>`_) information from GNSS modules which communicate
with `NMEA 0183 <https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard>`_ over serial.
It can retrieve GNSS data from any GNSS device sending
`NMEA 0183 <https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard>`_ sentences using serial communication.

*GNSS Interface* provides a lightweight and easy-to-use API with which applications can wait until
data of any of the supported `NMEA 0183 <https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard>`_ sentences is
received, and then retrieve it in an understandable manner without the need of knowing the inner details of the
`NMEA 0183 <https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard>`_ protocol.

The source code is hosted on `GitHub <https://github.com/EduPonz/gnss_interface>`_, check it out!
