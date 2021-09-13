.. _dev_docs_testing_infrastructure:

Testing Infrastructure
======================

This section documents the decisions made regarding the *GNSS Interface* testing infrastructure.

.. contents::
    :depth: 1
    :local:

.. _dev_docs_testing_framework:

Testing Framework
-----------------

The *GNSS Interface* testing framework has to cope with the following requirements:

1. Easy to integrate with CMake
1. Easy to integrate with GitHub actions
1. Large acceptance, so new contributors can write tests effortlessly
1. Mocking capabilities. This is because at least Asio will have to be mocked
1. Extense documentation
1. Easy to find answers to common problems.
1. Should be able to be used to create tests for the documentation

To satisfy these requirements *GNSS Interface* uses `Gtest <https://google.github.io/googletest/>`_ as testing
framework.
This decision is taken for a number or reasons:

1. Huge acceptance
1. Very large community, which means tons of Q&A everywhere
1. Very good documentation with examples
1. Out-of-the box mock support
1. Direct integration with CMake
1. GitHub integration merely consists on an action which installs GTest.

Other testing framework such as Catch and Boost.Test, however they were discarded:

* `Catch <https://github.com/catchorg/Catch2/tree/devel/docs>`_ seemed very promising, specially being a header only
  library, but the lack of mocking support is unfortunately a no-go for *GNSS Interface*.
* `Boost.Test <https://www.boost.org/doc/libs/1_75_0/libs/test/doc/html/index.html>`_, which also offers a header only
  version, but again, it does not have built-in mocking support.

.. _dev_docs_testing_cmake_options:

CMake Options
-------------

The *GNSS Interface* tests can be divided into two large categories:

1. Library tests: Unit and system tests for the *GNSS Interface* library itself.
1. Documentation tests: Automated tests for the documentation.

It should be possible to build these sets of tests separately, specially since not everyone would build the
documentation.
For that reason, 3 CMake options are added:

1. :class:`BUILD_LIBRARY_TESTS`: Builds the library tests
2. :class:`BUILD_DOCUMENTATION_TESTS`: Builds the documentation tests. This entails building the documentation.
3. :class:`BUILD_TESTS`: Builds all the *GNSS Interface* tests, meaning both library and documentation tests.

.. _dev_docs_testing_directories:

Directories
-----------

The *GNSS Interface* tests are held in the following directory structure:

1. `<repo-root>/test/unit`: For unit tests
1. `<repo-root>/test/system`: For system tests
1. `<repo-root>/docs/test`: For documentation tests
