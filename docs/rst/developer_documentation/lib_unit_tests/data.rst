.. include:: ../../include/aliases.rst

.. _unit_tests_nmea0183_data:

NMEA 0183 Data Types Unit Tests
===============================

As described in :ref:`dev_docs_libs_arch_api`, the way in which |EasyNmea-api| provides applications with NMEA data
is through the NMEA 0183 data types (|GPGGAData-api|).
These types feature ``==`` and ``!=`` operators, so that two samples of the same type can be compared between them.
Therefore, a set of unit tests for these operators of each of the types is required:

.. uml:: ../../../uml/data.puml

1. **NMEA0183DataComparisonOperators**: Checks that both comparison operators work for |NMEA0183Data-api|.
2. **GPGGADataComparisonOperators**: Checks that both comparison operators work for |GPGGAData-api|.
