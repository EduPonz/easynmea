.. include:: ../../include/aliases.rst

.. _unit_tests_opennmea:

OpenNmea Unit Tests
===================

As documented in :ref:`dev_docs_libs_arch_api`, |OpenNmea-api| provides applications with APIs to open and close
the serial port, wait until data of one or more NMEA 0183 types is received, check whether the serial port connection is
opened, and take the next unread sample of a given NMEA 0183 type.

The |OpenNmea-api| tests use the :class:`OpenNmeaTest` class, which derives from |OpenNmea-api|, adding
the possibility of substituting the |OpenNmeaImpl-api| with another instance.
This enables the tests to implement a :class:`OpenNmeaImplMock`, which derives from |OpenNmeaImpl-api|,
mocking away the |OpenNmeaImpl::open-api|, |OpenNmeaImpl::is_open-api|, |OpenNmeaImpl::close-api|,
|OpenNmeaImpl::wait_for_data-api|, and |OpenNmeaImpl::take_next-api| functions.
This way, the tests can substitute the |OpenNmeaImpl-api| instance in :class:`OpenNmeaTest` with an instance
of :class:`OpenNmeaImplMock` on which expectations can be set, and then check whether |OpenNmea-api| behaves
as expected depending on the |OpenNmeaImpl-api| returned values.

.. uml:: ../../../uml/OpenNmeaTests.puml

.. contents::
    :depth: 1
    :local:

.. _unit_tests_opennmea_open:

open()
------

1. **openOk**: Check that |OpenNmea::open-api| passes the correct arguments to |OpenNmeaImpl::open-api|, and
   that it returns |ReturnCode::RETURN_CODE_OK-api| whenever |OpenNmeaImpl::open-api| does so.
2. **openError**: Check that |OpenNmea::open-api| passes the correct arguments to |OpenNmeaImpl::open-api|, and
   that it returns |ReturnCode::RETURN_CODE_ERROR-api| whenever |OpenNmeaImpl::open-api| does so.
3. **openIllegal**: Check that |OpenNmea::open-api| passes the correct arguments to |OpenNmeaImpl::open-api|, and
   that it returns |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api| whenever |OpenNmeaImpl::open-api| does so.

.. _unit_tests_opennmea_is_open:

is_open()
---------

1. **is_openOpened**: Check that |OpenNmea::is_open-api| returns ``true`` when a connection is opened.
2. **is_openClosed**: Check that |OpenNmea::is_open-api| returns ``false`` when a connection is closed.

.. _unit_tests_opennmea_close:

close()
-------

1. **closeOk**: Check that |OpenNmea::close-api| returns |ReturnCode::RETURN_CODE_OK-api| when an opened port is
   closed correctly.
2. **closeError**: Check that |OpenNmea::close-api| returns |ReturnCode::RETURN_CODE_ERROR-api| when an opened port
   cannot be closed correctly.
3. **closeIllegal**: Check that |OpenNmea::close-api| returns |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api| when
   attempting to close an already closed port.

.. _unit_tests_opennmea_take_next:

take_next()
-----------

1. **take_nextOk**: Check that |OpenNmea::take_next-api| calls to |OpenNmeaImpl::take_next-api| with the
   appropriate arguments, and that if returns |ReturnCode::RETURN_CODE_OK-api| whenever
   |OpenNmeaImpl::take_next-api| does so.
   Furthermore, check that the data output is the sample output by |OpenNmeaImpl::take_next-api|.
2. **take_nextNoData**: Check that |OpenNmea::take_next-api| calls to |OpenNmeaImpl::take_next-api| with the
   appropriate arguments, and that if returns |ReturnCode::RETURN_CODE_OK-api| whenever
   |OpenNmeaImpl::take_next-api| does so.
   Furthermore, check that the data output is equal to the input.

.. _unit_tests_opennmea_wait_for_data:

wait_for_data()
---------------

1. **wait_for_dataOk**: Check that |OpenNmeaImpl::wait_for_data-api| is called with the appropriate arguments, and
   that |OpenNmea::wait_for_data-api| returns |ReturnCode::RETURN_CODE_OK-api| whenever
   |OpenNmeaImpl::wait_for_data-api| does so.
2. **wait_for_dataTimeout**: Check that |OpenNmeaImpl::wait_for_data-api| is called with the appropriate arguments,
   and that |OpenNmea::wait_for_data-api| returns |ReturnCode::RETURN_CODE_TIMEOUT-api| whenever
   |OpenNmeaImpl::wait_for_data-api| does so.
3. **wait_for_dataTimeoutDefault**: The difference with **wait_for_dataTimeout** os that in this case,
   |OpenNmea::wait_for_data-api| is called leaving the timeout as default.
4. **wait_for_dataIllegal**: Check that |OpenNmeaImpl::wait_for_data-api| is called with the appropriate arguments,
   and that |OpenNmea::wait_for_data-api| returns |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api| whenever
   |OpenNmeaImpl::wait_for_data-api| does so.
5. **wait_for_dataError**: Check that |OpenNmeaImpl::wait_for_data-api| is called with the appropriate arguments,
   and that |OpenNmea::wait_for_data-api| returns |ReturnCode::RETURN_CODE_ERROR-api| whenever
   |OpenNmeaImpl::wait_for_data-api| does so.
