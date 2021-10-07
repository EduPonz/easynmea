.. include:: ../include/aliases.rst

.. _usage:

Usage
=====

*EasyNMEA* provides the |EasyNmea-api| class, which uses
`NMEA 0183 <https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard>`_ sentences to extract **NMEA** information from
the NMEA devices.
It provides an easy-to-use API with which applications can open a serial communication channel with the NMEA device,
wait until some data from one or more `NMEA 0183 <https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard>`_
sentences arrives, retrieve it and digest it in an understandable manner, and close the connection.

The following snippet shows how to use |EasyNmea::open-api|, |EasyNmea::wait_for_data-api|,
|EasyNmea::take_next-api|, and |EasyNmea::close-api| APIs to wait until |GPGGAData-api| data is received,
using a |NMEA0183DataKindMask-api| set to |NMEA0183DataKind::GPGGA-api|. For more information about the supported
`NMEA 0183 <https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard>`_ sentences and their meaning, please refer to
:ref:`nmea_data_types`.

.. literalinclude:: /rst/snippets/snippets.cpp
   :language: c++
   :start-after: //USAGE_BASIC
   :end-before: //!--
   :dedent: 8
