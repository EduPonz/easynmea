.. _dev_docs_system_tests:

System Tests
============

*OpenNMEA* provides a set of test which execute end-to-end verification of the library's functionalities.
This is done by simulating a NMEA device sending data to a serial port.
This data is then received by a *OpenNMEA* application which uses the library's public API to open the serial
connection, wait until data of any given kind is received, and log this data for validation against expectations.
For connecting the NMEA device double and the *OpenNMEA* application,
`socat <http://www.dest-unreach.org/socat/>`_ is used to create a pair of **virtual serial ports**, one for the double
to send the data, and the other one for the application to receive it.
This way, the *OpenNMEA* application acts in the same way as a real application would, so public APIs can be
tested in the same manner that they would be used in real applications.
The relationships between the different system test components and the sequence of operations are shown in the following
diagrams.

.. uml:: ../../../uml/system_tests_arch.puml

.. uml:: ../../../uml/system_tests_arch_sequence.puml

.. contents::
    :depth: 1
    :local:


1. **gpgga_read_some_and_close**: Open a pair of serial ports, send some valid NMEA sentences in one, and read GPGGA
   data on the other.
   Then, first close the OpenNMEA and then close the ports.
   Validate results against expectations.
2. **port_closed_externally**: Open a pair of serial ports, send some valid NMEA sentences in one, and read GPGGA data
   on the other.
   Then, close the serial ports with the OpenNMEA still opened.
   The application should detect this an exist gracefully.
   Validate results against expectations.
3. **stop_sending_data**: Open a pair of serial ports, send some valid NMEA sentences in one, and read GPGGA data on the
   other.
   Stop sending data before stopping the OpenNMEA.
   Close the OpenNMEA, then the sending app, and lastly close the ports.
   Validate results against expectations.
4. **late_sending**: Open a pair of serial ports.
   Then, first start a OpenNMEA, and after 1 second start sending some valid NMEA sentences.
   Then, close the OpenNMEA before closing the ports.
   Validate results against expectations.
