.. include:: ../include/aliases.rst

.. _dev_docs_library_architecture:

Library Architecture
====================

*EasyNMEA* is divided into three levels (from outer to inner):

1. :ref:`dev_docs_libs_arch_api` : This level contains all public API, i.e. the classes in the ``include`` directory.
2. :ref:`dev_docs_libs_arch_impl`: This level contains all the internal classes which provide functionality to the
   library.
3. :ref:`dev_docs_libs_arch_serial`: This level contains the classes for interacting with the serial port (through
   Asio).

.. uml:: ../../uml/library_levels.puml

.. _dev_docs_libs_arch_api:

API Level
---------

The API level comprises all the *EasyNMEA* public classes and structures, and acts as entry point for the
library's functionalities.
It consists of a main class |EasyNmea-api|, which provides application with access to the functionalities, and all
the supporting classes and structures for return types and input and output parameters.
Those companion classes and structures are |ReturnCode-api|, |GPGGAData-api|, and |NMEA0183DataKindMask-api|.
For the actual functionality implementation, |EasyNmea-api| relies on the internal class |EasyNmeaImpl-api|.

.. uml:: ../../uml/EasyNmea.puml

.. _dev_docs_libs_arch_impl:

Implementation Level
--------------------

The implementation level's main component is the |EasyNmeaImpl-api| class, which provides with implementation for
the |EasyNmea-api| public API, i.e opening and closing the serial port, waiting until data of one or more NMEA 0183
types has been received, checking whether the serial port connection is opened, and taking the next unread sample of a
given NMEA 0183 type.
The |EasyNmeaImpl-api| holds a |FixedSizeQueue-api| of ten elements for each supported
NMEA 0183 type.
This way, keeping outdated samples, as well as dynamic allocation of data samples, is avoided.
The managing of the serial port is enable through the |SerialInterface-api| class.

.. uml:: ../../uml/EasyNmeaImpl.puml

.. _dev_docs_libs_arch_serial:

Serial Interface Level
----------------------

The serial interface level is comprised of the |SerialInterface-api| class, which provides member functions to open and
close a serial port, as well as for reading data from it.
|SerialInterface-api| is a template class with a template parameter :class:`SerialPort` that defines the serial port
implementation, which defaults to :class: :class:`asio::serial_port`.

.. uml:: ../../uml/SerialInterface.puml
