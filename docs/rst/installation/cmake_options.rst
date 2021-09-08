.. include:: ../include/aliases.rst

.. _installation_cmake_options:

CMake Options
-------------

*GNSS Interface* provides several CMake options that can be used to build or exclude certain library modules.

.. list-table::
    :header-rows: 1

    *   - Option
        - Description
        - Possible values
        - Default
    *   - :class:`BUILD_DOCUMENTATION`
        - Generates Doxygen and Sphinx |br|
          documentation (see |br|
          :ref:`installation_documentation`)
        - ``ON`` | ``OFF``
        - ``OFF``
    *   - :class:`BUILD_EXAMPLES`
        - Builds *GNSS Interface* examples
        - ``ON`` | ``OFF``
        - ``OFF``
