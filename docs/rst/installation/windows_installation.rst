.. _installation_windows:

Build and Install on Windows
============================

This guide describes the process of building and installing *GNSS Interface* on Windows platforms:

.. contents::
    :local:
    :backlinks: none
    :depth: 2

.. _installation_windows_prerequisites:

Prerequisites
-------------

To build and install *GNSS Interface*, some external tools are required.

* `CMake <https://cmake.org/>`_
* `Visual Studio <https://visualstudio.microsoft.com/>`_
* `Wget <https://www.gnu.org/software/wget/>`_
* `Git <https://git-scm.com/>`_
* `Chocolatey <https://chocolatey.org/>`_
* `pip3 <https://docs.python.org/3/installing/index.html>`_

.. _installation_windows_dependencies:

Dependencies
------------

*Gnss Interface* depends on `Asio <https://think-async.com/Asio/>`_, a cross-platform C++ library for network and
low-level I/O programming that provides a consistent asynchronous model, which is used to interact with the serial
ports.
Chocolatey can be used to install Asio on Windows platforms.
Download the `package <https://github.com/ros2/choco-packages/releases/download/2020-02-24/asio.1.12.1.nupkg>`_ and run:

.. code-block:: bash

    choco install -y -s <download_dir> asio

Where :code:`<download_dir>` is the directory into which the package has been downloaded.

.. _installation_windows_build:

Build
-----

Once the :ref:`prerequisites<installation_windows_prerequisites>` and
:ref:`dependencies<installation_windows_dependencies>` are installed, *GNSS Interface* can be built with CMake by
running:

.. code-block:: bash

    cd ~
    git clone https://github.com/EduPonz/gnss_interface.git
    cd gnss_interface
    mkdir build && cd build
    cmake ..
    cmake --build .

.. note::

    For more information about compilation options please refer to :ref:`installation_cmake_options`.

.. _installation_windows_install:

Install
-------

Once the library is built, in can be installed in a user specified directory with:

.. code-block:: bash

    cd ~/gnss_interface/build
    cmake .. -DCMAKE_INSTALL_PREFIX=<user-specified-dir>
    cmake --build . --target install

Alternatively, it can also be installed system-wide with:

.. code-block:: bash

    cd ~/gnss_interface/build
    cmake ..
    cmake --build . --target install
