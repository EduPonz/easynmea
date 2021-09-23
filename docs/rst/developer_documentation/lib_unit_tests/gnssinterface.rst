.. include:: ../../include/aliases.rst

.. _unit_tests_gnssinterface:

GnssInterface Unit Tests
========================

As documented in :ref:`dev_docs_libs_arch_api`, |GnssInterface-api| provides applications with APIs to open and close
the serial port, wait until data of one or more NMEA 0183 types is received, check whether the serial port connection is
opened, and take the next unread sample of a given NMEA 0183 type.

The |GnssInterface-api| tests use the :class:`GnssInterfaceTest` class, which derives from |GnssInterface-api|, adding
the possibility of substituting the |GnssInterfaceImpl-api| with another instance.
This enables the tests to implement a :class:`GnssInterfaceImplMock`, which derives from |GnssInterfaceImpl-api|,
mocking away the |GnssInterfaceImpl::open-api|, |GnssInterfaceImpl::is_open-api|, |GnssInterfaceImpl::close-api|,
|GnssInterfaceImpl::wait_for_data-api|, and |GnssInterfaceImpl::take_next-api| functions.
This way, the tests can substitute the |GnssInterfaceImpl-api| instance in :class:`GnssInterfaceTest` with an instance
of :class:`GnssInterfaceImplMock` on which expectations can be set, and then check whether |GnssInterface-api| behaves
as expected depending on the |GnssInterfaceImpl-api| returned values.

.. uml:: ../../../uml/GnssInterfaceTests.puml

.. contents::
    :depth: 1
    :local:

.. _unit_tests_gnssinterface_open:

open()
------

1. **openOk**: Check that |GnssInterface::open-api| passes the correct arguments to |GnssInterfaceImpl::open-api|, and
   that it returns |ReturnCode::RETURN_CODE_OK-api| whenever |GnssInterfaceImpl::open-api| does so.
2. **openError**: Check that |GnssInterface::open-api| passes the correct arguments to |GnssInterfaceImpl::open-api|, and
   that it returns |ReturnCode::RETURN_CODE_ERROR-api| whenever |GnssInterfaceImpl::open-api| does so.
3. **openIllegal**: Check that |GnssInterface::open-api| passes the correct arguments to |GnssInterfaceImpl::open-api|, and
   that it returns |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api| whenever |GnssInterfaceImpl::open-api| does so.

.. _unit_tests_gnssinterface_is_open:

is_open()
---------

1. **is_openOpened**: Check that |GnssInterface::is_open-api| returns ``true`` when a connection is opened.
2. **is_openClosed**: Check that |GnssInterface::is_open-api| returns ``false`` when a connection is closed.

.. _unit_tests_gnssinterface_close:

close()
-------

1. **closeOk**: Check that |GnssInterface::close-api| returns |ReturnCode::RETURN_CODE_OK-api| when an opened port is
   closed correctly.
2. **closeError**: Check that |GnssInterface::close-api| returns |ReturnCode::RETURN_CODE_ERROR-api| when an opened port
   cannot be closed correctly.
3. **closeIllegal**: Check that |GnssInterface::close-api| returns |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api| when
   attempting to close an already closed port.

.. _unit_tests_gnssinterface_take_next:

take_next()
-----------

1. **take_nextOk**: Check that |GnssInterface::take_next-api| calls to |GnssInterfaceImpl::take_next-api| with the
   appropriate arguments, and that if returns |ReturnCode::RETURN_CODE_OK-api| whenever
   |GnssInterfaceImpl::take_next-api| does so.
   Furthermore, check that the data output is the sample output by |GnssInterfaceImpl::take_next-api|.
2. **take_nextNoData**: Check that |GnssInterface::take_next-api| calls to |GnssInterfaceImpl::take_next-api| with the
   appropriate arguments, and that if returns |ReturnCode::RETURN_CODE_OK-api| whenever
   |GnssInterfaceImpl::take_next-api| does so.
   Furthermore, check that the data output is equal to the input.

.. _unit_tests_gnssinterface_wait_for_data:

wait_for_data()
---------------

1. **wait_for_dataOk**: Check that |GnssInterfaceImpl::wait_for_data-api| is called with the appropriate arguments, and
   that |GnssInterface::wait_for_data-api| returns |ReturnCode::RETURN_CODE_OK-api| whenever
   |GnssInterfaceImpl::wait_for_data-api| does so.
2. **wait_for_dataTimeout**: Check that |GnssInterfaceImpl::wait_for_data-api| is called with the appropriate arguments,
   and that |GnssInterface::wait_for_data-api| returns |ReturnCode::RETURN_CODE_TIMEOUT-api| whenever
   |GnssInterfaceImpl::wait_for_data-api| does so.
3. **wait_for_dataTimeoutDefault**: The difference with **wait_for_dataTimeout** os that in this case,
   |GnssInterface::wait_for_data-api| is called leaving the timeout as default.
4. **wait_for_dataIllegal**: Check that |GnssInterfaceImpl::wait_for_data-api| is called with the appropriate arguments,
   and that |GnssInterface::wait_for_data-api| returns |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api| whenever
   |GnssInterfaceImpl::wait_for_data-api| does so.
5. **wait_for_dataError**: Check that |GnssInterfaceImpl::wait_for_data-api| is called with the appropriate arguments,
   and that |GnssInterface::wait_for_data-api| returns |ReturnCode::RETURN_CODE_ERROR-api| whenever
   |GnssInterfaceImpl::wait_for_data-api| does so.
