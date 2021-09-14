.. include:: ../include/aliases.rst

.. _dev_docs_library_architecture:

Library Architecture
====================

.. warning::

    This section is under construction, so information might be incomplete or missing.

*GNSS Interface* is divided into three levels (from outer to inner):

1. **API level**: This level contains all public API, i.e. the classes in the ``include`` directory.
2. **Implementation level**: This level contains all the internal classes which provide functionality to the library.
3. **Serial interface level**: This level contains the classes for interacting with the serial port (through Asio)


.. _dev_docs_libs_arch_serial:

Serial Interface Level
----------------------

The serial interface level is comprised of the |SerialInterface-api| class, which provides member functions to open and
close a serial port, as well as for reading data from it.
|SerialInterface-api| contains one :class:`asio::serial_port` which is used to interact with the port.

.. uml:: ../../uml/SerialInterface.puml
