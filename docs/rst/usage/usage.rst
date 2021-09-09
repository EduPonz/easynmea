.. include:: ../include/aliases.rst

.. _usage:

Usage
=====

*GNSS Interface* provides the :class:`GnssInterface` class, which uses **NMEA 0183** sentences to extract **GNSS**
information from the GNSS devices.
It provides an easy-to-use API with which applications can open a serial communication channel with the GNSS device,
wait until some data from one or more **NMEA 0183** sentences arrives, retrieve it in a digested and understandable
manner, and close the connection.

The following snippet shows how to use |GnssInterface::open-api|, |GnssInterface::wait_for_data-api|,
|GnssInterface::take_next-api|, and |GnssInterface::close-api| APIs, to wait until |NMEA0183DataKind::GPGGA-api| data is
received, using a |NMEA0183DataKindMask-api|. For more information about the supported **NMEA 0183** sentences and their
meaning please refer to :ref:`nmea_data_types`.

.. literalinclude:: /rst/snippets/snippets.cpp
   :language: c++
   :start-after: //USAGE_BASIC
   :end-before: //!--
   :dedent: 8
