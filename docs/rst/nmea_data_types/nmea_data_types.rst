.. include:: ../include/aliases.rst

.. _nmea_data_types:

NMEA 0183 Data Types
====================

This section presents the data types associated with the
`NMEA 0183 <https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard>`_ sentences that are interpreted by *GNSS
Interface*.

.. _nmea_data_types_gpgga:

GPGGA
-----

The |GPGGAData-api| provides **Global Positioning System Fix Data**, meaning that it is advertised only
when the GNSS device has been able to acquire a fix.
The |GPGGAData-api| provides information about:

* **Timestamp**; always in `hhmmss.milliseconds`.
* **Latitude**; always in degrees referred to North.
* **Longitude**; always in degrees referred to East.
* **Fix**: whether there is a fix position. 0 means no fix, 1 means fix, and 2 means differential fix.
* **Satellites on view**: Number of satellites that the GNSS device can see.
* **Horizontal precision**; always in meters.
* **Altitude over sea level**; always in meters.
