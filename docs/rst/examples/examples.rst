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
using the **NMEA 0183** GPGGA sentence.
Once the examples :ref:`have been built <examples_build>`, the GPGGA example can be run with:

.. code:: bash

    cd <user-specified-dir>/examples/bin
    ./gpgga_example --serial_port /dev/ttyACM0 --baudrate 9600
