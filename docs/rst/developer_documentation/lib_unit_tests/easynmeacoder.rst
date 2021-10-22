.. include:: ../../include/aliases.rst

.. _unit_tests_easynmeacoder:

EasyNmeaCoder Unit Tests
========================

As documented in :ref:`dev_docs_libs_arch_impl`, |EasyNmeaCoder-api| provides APIs for decoding NMEA 0183 supported
sentences, specifically |EasyNmeaCoder::decode-api|.
This member function takes a NMEA 0183 sentence as a string and returns a :class:`std::shared_ptr` to a
|NMEA0183Data-api|, which |NMEA0183DataKind-api| field can be used to cast it into the appropriate NMEA 0183 data
structure.
This set of tests target the |EasyNmeaCoder::decode-api| function, passing different sentences and checking the return
against expected outputs.

.. uml:: ../../../uml/EasyNmeaCoderTests.puml

.. contents::
    :depth: 1
    :local:

.. _unit_tests_easynmeacoder_decode:

decode()
--------

1. **decodeGPGGAValidNE**
2. **decodeGPGGAValidNW**
3. **decodeGPGGAValidSE**
4. **decodeGPGGAValidSW**
5. **decodeGPGGAValidNoAgeOfDiffGPS**
6. **decodeGPGGAValidEmptyAgeOfDiffGPSNoDiffRefStation**
7. **decodeGPGGAValidNoDiffRefStation**
8. **decodeGPGGAValidNoOptionals**
9.  **decodeGPGGAInvalidTime**
10. **decodeGPGGAInvalidLatitudeLength**
11. **decodeGPGGAInvalidLatitudeDegrees**
12. **decodeGPGGAInvalidLatitudeMinutes**
13. **decodeGPGGAInvalidLongitudeLength**
14. **decodeGPGGAInvalidLongitudeDegrees**
15. **decodeGPGGAInvalidLongitudeMinutes**
16. **decodeGPGGAInvalidAltitudeUnits**
17. **decodeGPGGAInvalidHeightUnits**
18. **decodeGPGGAInvalidChecksum**
19. **decodeGPGGANoTime**
20. **decodeGPGGANoLatitude**
21. **decodeGPGGANoLongitude**
22. **decodeGPGGANoFix**
23. **decodeGPGGANoNumberOfSatellites**
24. **decodeGPGGANoHDOP**
25. **decodeGPGGANoAltitude**
26. **decodeGPGGANoHeight**
27. **decodeGPGGANoChecksum**
28. **decodeInvalidSentenceID**
29. **decodeUnsupportedSentence**
30. **decodeEmptySentence**
31. **decodeOnlyChecksumSentence**
32. **decodeOnlyAstheriscSentence**
