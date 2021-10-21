.. _dev_docs_unit_tests:

Unit Tests
==========

*EasyNMEA* provides one test suite containing unit tests for each of the library classes.
These suits test each and every public member function separately, mocking lower levels so that every possible case can
be covered.

Even while the test suites provide a 100% line coverage on the classes they test, a 100% branch coverage is not
required, as the implementation may use external functions that are not marked as ``noexcept``, for which the compiler
may generate branches that are virtually impossible to hit.
It is up to the reviewers and maintainers to judge whether the branch coverage of a specific contribution is high
enough, or if more test cases are required.

.. toctree::
   :maxdepth: 1

   /rst/developer_documentation/lib_unit_tests/data
   /rst/developer_documentation/lib_unit_tests/easynmea
   /rst/developer_documentation/lib_unit_tests/easynmeacoder
   /rst/developer_documentation/lib_unit_tests/easynmeaimpl
   /rst/developer_documentation/lib_unit_tests/serialinterface
