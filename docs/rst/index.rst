GNSS Interface Documentation
============================

.. toctree::
   :maxdepth: 3
   :hidden:

   installation/installation
   usage
   api_reference

*GNSS Interface* is a cross-platform `C++` library to retrieve **GNSS** information from GNSS
modules which communicate with **NMEA 0183** over serial.
It has been originally developed targeting Quectel GNSS L86 modules, however, it can retrieve GNSS
data from any GNSS device sending **NMEA 0183** sentences using serial communication.

*GNSS Interface* provides a lightweight and easy-to-use API with which applications can wait until
data of any of the supported NMEA 0183 sentences is received, and then retrieve it in an
understandable manner without the need of knowing the inner details of the NMEA 0813 protocol.
