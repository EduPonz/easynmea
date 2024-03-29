.. _installation_ubuntu:

Build and Install on Ubuntu
===========================

This guide describes the process of building and installing *EasyNMEA* on Ubuntu platforms:

.. contents::
    :local:
    :backlinks: none
    :depth: 2

.. _installation_ubuntu_prerequisites:

Prerequisites
-------------

To build and install *EasyNMEA*, some external tools are required.

.. code:: bash

    sudo apt update && sudo apt install -y \
        cmake \
        g++ \
        wget \
        git \
        python3-pip

.. _installation_ubuntu_dependencies:

Dependencies
------------

*EasyNMEA* depends on `Asio <https://think-async.com/Asio/>`_, a cross-platform C++ library for network and
low-level I/O programming that provides a consistent asynchronous model, which is used to interact with the serial
ports.
This can be installed with:

.. code:: bash

    sudo apt update && sudo apt install -y libasio-dev

.. _installation_ubuntu_build:

Build
-----

Once the :ref:`prerequisites<installation_ubuntu_prerequisites>` and
:ref:`dependencies<installation_ubuntu_dependencies>` are installed, *EasyNMEA* can be built with the help of
CMake by running:

.. code-block:: bash

    cd ~
    git clone https://github.com/EduPonz/easynmea.git
    cd easynmea
    mkdir build && cd build
    cmake ..
    cmake --build .

.. note::

    For more information about compilation options please refer to :ref:`installation_cmake_options`.

.. _installation_ubuntu_install:

Install
-------

Once the library is built, in can be installed in a user specified directory with:

.. code-block:: bash

    cd ~/easynmea/build
    cmake .. -DCMAKE_INSTALL_PREFIX=<user-specified-dir>
    cmake --build . --target install

Alternatively, it can also be installed system-wide with:

.. code-block:: bash

    cd ~/easynmea/build
    cmake ..
    cmake --build . --target install
