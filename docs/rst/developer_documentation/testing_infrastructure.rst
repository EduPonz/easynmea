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
2. Easy to integrate with GitHub actions
3. Large acceptance, so new contributors can write tests effortlessly
4. Mocking capabilities. This is because at least Asio will have to be mocked
5. Extense documentation
6. Easy to find answers to common problems.
7. Should be able to be used to create tests for the documentation

To satisfy these requirements *GNSS Interface* uses `Gtest <https://google.github.io/googletest/>`_ as testing
framework.
This decision is taken for a number or reasons:

1. Huge acceptance
2.  Very large community, which means tons of Q&A everywhere
3. Very good documentation with examples
4. Out-of-the box mock support
5. Direct integration with CMake
6. GitHub integration merely consists on an action which installs GTest.

Other testing framework such as Catch and Boost.Test, however they were discarded:

* `Catch <https://github.com/catchorg/Catch2/tree/devel/docs>`_ seemed very promising, specially being a header only
  library, but the lack of mocking support is unfortunately a no-go for *GNSS Interface*.
* `Boost.Test <https://www.boost.org/doc/libs/1_75_0/libs/test/doc/html/index.html>`_, which also offers a header only
  version, but again, it does not have built-in mocking support.

.. _dev_docs_testing_build_tests:

Build Tests
-----------

The *GNSS Interface* tests can be divided into two large categories:

1. **Library tests**: Unit and system tests for the *GNSS Interface* library itself.
2. **Documentation tests**: Automated tests for the documentation.

Although none of these tests are built by default, it is possible to build them separately.
This is because not everyone would build the documentation.
To do that, 3 CMake options are added:

1. :class:`BUILD_LIBRARY_TESTS`: Builds the library tests
2. :class:`BUILD_DOCUMENTATION_TESTS`: Builds the documentation tests. This entails building the documentation.
3. :class:`BUILD_TESTS`: Builds all the *GNSS Interface* tests, meaning both library and documentation tests.

Furthermore, the system tests within the **Library tests** do require the installation of some extra python
dependencies, which are listed in `<path_to_repo>/test/system/requirements.txt`.
These are necessary to simulate a serial connection and a GNSS device.
They can be installed with:

.. code:: bash

    python3 install -r <path_to_repo>/test/system/requirements.txt

.. _dev_docs_testing_directories:

Directories
-----------

The *GNSS Interface* tests are held in the following directory structure:

1. ``<repo-root>/test/unit``: For unit tests
2. ``<repo-root>/test/system``: For system tests
3. ``<repo-root>/docs/test``: For documentation tests

.. _dev_docs_testing_automated:

Automated Testing Jobs
----------------------

All the *GNSS Interface* tests run automatically once a day for the :class:`main` branch, as well as for the supported
versions' branches.
Furthermore, all the tests are run whenever a pull request is opened and with every commit pushed to an open pull
request.
To automate these tasks, since the public repository is hosted on GitHub,
`GitHub actions <https://github.com/features/actions>`_ are used.
This tool enables to create as many workflows with as many jobs in them as desired, making it ideal for test automation.
Moreover, the jobs run on GitHub maintained servers, so the only thing we have to do is to define those workflows.
This is done in ``<repo-root>/.github/workflows``.
*GNSS Interface* contains the following workflows and jobs:

* :class:`automated_testing`, defined in ``<repo-root>/.github/workflows/automated_testing.yml``.
  This workflow runs on pushes to :class:`main` and any other maintained branch, on pull request creation or update,
  and once a day.
  It contains the following jobs:

   * :class:`ubuntu-build-test`, which runs in the latest Ubuntu distribution available.
     This job installs all the necessary dependencies, builds all the tests and documentation, runs the all tests, and
     uploads the sphinx-generated HTML documentation so reviewers can check it.

.. _dev_docs_coverage:

Code Coverage Reporting
-----------------------

As stated in :ref:`dev_docs_testing_automated`, *GNSS Interface* tests are run with every push to :class:`main` and
supported version branches, as well as with every push to any open pull request.
This is done to make sure that every aspect of the library works as expected, as well as to guarantee that new changes
do not break any established behaviour.
Code coverage reporting takes this a step further, not only guaranteeing that all the tests pass at all times, but also
checking whether those tests reach every possible source code outcome.

This is done using compiler specific flags that report every branch generated by the compiler and reached by the tests.
These reports are then gather under one single human-readable code coverage report that is uploaded to an online
platform, which in turn can keep track of the coverage progress with changes.

Presently, the coverage reports are generated in the :class:`ubuntu-build-test` job, passing specific flags to
`GCC <https://gcc.gnu.org/>`_.
Those flags are: ``--coverage``, ``-fprofile-arcs``, and ``-ftest-coverage``.
To ease the compilation, a CMake option :class:`GCC_CODE_COVERAGE` has been created, which enables the code coverage
flags if the compiler used is indeed GCC.

Then, the job uses `gcovr <https://gcovr.com/en/stable/>`_ to generate a report that is uploaded to
`Codecov <https://app.codecov.io/gh/EduPonz/gnss_interface/>`_.
In turn, Codecov checks the code coverage on the changes proposed in the pull request, as well as the overall coverage.
If any of those two decreases, the code coverage check fails, and the pull request cannot be merged.

.. _dev_docs_codeql:

Code Quality Analysis
---------------------

With every push to :class:`main`, and with every pull request targeting it, and automated job is run to check code
vulnerabilities using `CodeQL <https://codeql.github.com/>`_.
This job presents vulnerabilities in the form of code scanning alerts (see
`About code scanning with CodeQL <https://docs.github.com/en/code-security/code-scanning/automatically-scanning-your-code-for-vulnerabilities-and-errors/about-code-scanning-with-codeql>`_).
