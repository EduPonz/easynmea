.. _installation_documentation:

Build and Install Documentation
-------------------------------

.. important::

    This guide assumes that the library as been built following the steps outlined in :ref:`installation_ubuntu`.
    Else, paths might need to be adjusted to align with the followed procedure.

*GNSS Interface's* documentation is comprised  of `Doxygen <https://www.doxygen.nl/index.html>`_ and
`Sphinx <https://www.sphinx-doc.org/en/master/>`_ HTML output.
The process of building the documentation entails installation of additional tools for both the Doxygen and Sphinx
documentations.

.. _installation_documentation_setup:

Environment Setup
^^^^^^^^^^^^^^^^^

To ease the development process, and to avoid version incompatibilities or clashes, this guide describes the process of
building the documentation using `Python3 Virtual Environments <https://docs.python.org/3/tutorial/venv.html>`_.
Before setting up the environment, *Doxygen* needs to be installed.
Install *venv* and *Doxygen*, and create a virtual environment and install the necessary tools with:

.. code:: bash

    cd ~
    sudo apt update && sudo apt install -y python3-venv doxygen
    python3 -m venv gnss_interface_venv
    source gnss_interface_venv/bin/activate
    pip3 install -r ~/gnss_interface/docs/requirements.txt

.. _installation_documentation_build:

Build
^^^^^

After :ref:`setting up the environment <installation_documentation_setup>`, the documentation can be built with:

.. code:: bash

    source ~/gnss_interface_venv/bin/activate
    cd ~/gnss_interface/build
    cmake .. -DBUILD_DOCUMENTATION=ON -DCMAKE_INSTALL_PREFIX=<user-specified-dir>
    cmake --build .

.. _installation_documentation_install:

Install
^^^^^^^

After :ref:`building the documentation <installation_documentation_build>`, it can be installed with:

.. code:: bash

    source ~/gnss_interface_venv/bin/activate
    cd ~/gnss_interface/build
    cmake --build . --target install

Simulate Read The Docs Build
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To simulate the process followed on the `Read The Docs <https://readthedocs.org/>` to build this documentation, run:

.. code:: bash

    source ~/gnss_interface_venv/bin/activate
    cd ~/gnss_interface
    rm -rf build  # Just in case
    READTHEDOCS=True sphinx-build \
        -b html \
        -D breathe_projects.gnss_interface=<abs_path_to_docs_repo>/build/docs/doxygen/xml \
        -d <abs_path_to_docs_repo>/build/docs/doctrees \
        docs <abs_path_to_docs_repo>/build/docs/sphinx/html
