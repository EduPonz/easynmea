.. include:: ../include/aliases.rst

.. _installation_cmake_options:

CMake Options
-------------

*OpenNMEA* provides several CMake options that can be used to build or exclude certain library modules.

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
    *   - :class:`BUILD_LIBRARY_TESTS`
        - Build the library tests.
        - ``ON`` ``OFF``
        - ``OFF``
    *   - :class:`BUILD_DOCUMENTATION_TESTS`
        - Build the library documentation |br|
          tests. Setting this ``ON`` will set |br|
          :class:`BUILD_DOCUMENTATION` to ``ON``
        - ``ON`` ``OFF``
        - ``OFF``
    *   - :class:`BUILD_TESTS`
        - Build the library and |br|
          documentation tests. Setting |br|
          this ``ON`` will set |br|
          :class:`BUILD_LIBRARY_TESTS` and |br|
          :class:`BUILD_DOCUMENTATION_TESTS` |br|
          to ``ON``
        - ``ON`` ``OFF``
        - ``OFF``
    *   - :class:`BUILD_EXAMPLES`
        - Builds *OpenNMEA* examples
        - ``ON`` | ``OFF``
        - ``OFF``
    *   - :class:`GCC_CODE_COVERAGE`
        - Build the library with |br|
          code coverage support. |br|
          This flag only take action |br|
          when using GCC.
        - ``ON`` ``OFF``
        - ``OFF``
