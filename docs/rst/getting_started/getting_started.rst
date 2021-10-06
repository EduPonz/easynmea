.. _getting_started:

Getting Started
===============

Before doing anything else, you can get a flavor of the *OpenNMEA* capabilities by checking out the
`opennmea <https://hub.docker.com/repository/docker/eduponz/opennmea>`_ `Docker <https://www.docker.com/>`_
image for Ubuntu.
This image ships an already built *OpenNMEA* with compiled examples that you can use to get some readings out of
your NMEA sensor without building anything on your side.
If you do not have the Docker Engine already installed, you can install it following
`this tutorial <https://docs.docker.com/engine/install/ubuntu/>`_.
Then, there are two options for running the container:

.. contents::
    :local:
    :backlinks: none
    :depth: 2

.. _getting_started_run_docker:

Run docker knowing the specific serial device
----------------------------------------------

If your NMEA module is already connected to a serial port and it is not going to be unplugged, then you can just share
that device with the container:

.. code:: bash

    docker run -it --device=<path_to_device> eduponz/opennmea bash

Then, inside the container, you can run the GPGGA example with:

.. code:: bash

    /root/opennmea/build/examples/gpgga_example -b <baudrate> -p <path_to_device>

.. _getting_started_run_docker_plug:

Run docker allowing for plug-unplug connectivity
-------------------------------------------------

If your module may be unplug and plug while the container is running, you can still share the serial port with the
Docker container by sharing all the devices of the same cgroup.
Plug your device and get its cgroup with:

.. code:: bash

    ls -l <path_to_device> | awk '{print substr($5, 1, length($5)-1)}'

Then, run the container:

.. code:: bash

    docker run -it -v /dev:/dev --device-cgroup-rule='c <device cgroup>:* rmw' eduponz/opennmea bash

Finally, inside the container, you can run the GPGGA example as before:

.. code:: bash

    /root/opennmea/build/examples/gpgga_example -b <baudrate> -p <path_to_device>
