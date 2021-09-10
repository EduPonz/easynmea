.. _examples:

Build and Run Examples
======================

This page presents how to build and run all the
`GNSS Interface examples <https://github.com/EduPonz/gnss_interface/tree/main/examples>`_, as well as showcasing
sample outputs.

.. _examples_build:

Build Examples
--------------

.. note::

    This section assumes that the guides outlined in :ref:`installation` have been followed.

Building the *GNSS Interface* examples is as easy as add the CMake option ``-DBUILD_EXAMPLES=ON`` on CMake's
configuration step:

.. code:: bash

    cd ~/gnss_interface/build
    cmake .. -DCMAKE_INSTALL_PREFIX=<user-specified-dir> -DBUILD_EXAMPLES=ON
    cmake --build . --target install

.. _examples_gpgga:

GPGGA Example
-------------

The GPGGA example showcases how to get Global Positioning System Fix Data out of GNSS devices, which they advertise
using the `NMEA 0183 <https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard>`_ GPGGA sentence.
Once the examples :ref:`have been built <examples_build>`, the GPGGA example can be run with:

.. code:: bash

    cd <user-specified-dir>/examples/bin
    ./gpgga_example --serial_port /dev/ttyACM0 --baudrate 9600

An output example from `gpgga_example` would be:

.. code::

    Serial port '/dev/ttyACM0' opened. Baudrate: 9600
    Please press CTRL-C to stop the example

    ************** NEW GPGGA SAMPLE **************
    Elapsed time ---------> 3468
    ------------------------------------------
    GPGGA Data - GNSS Position Fix
    ==============================
    Message --------------> $GPGGA,072706.000,5703.1740,N,00954.9459,E,1,8,1.28,-21.2,M,42.5,M,,*4E
    Timestamp ------------> 72706
    Latitude -------------> 57.0529º N
    Longitude ------------> 9.91576º E
    Fix ------------------> 1
    Number of satellites -> 8
    Horizontal precision -> 1.28
    Altitude -------------> -21.2
