.. include:: ../../include/aliases.rst

.. _unit_tests_easynmea:

EasyNmea Unit Tests
===================

As documented in :ref:`dev_docs_libs_arch_api`, |EasyNmea-api| provides applications with APIs to open and close
the serial port, wait until data of one or more NMEA 0183 types is received, check whether the serial port connection is
opened, and take the next unread sample of a given NMEA 0183 type.

The |EasyNmea-api| tests use the :class:`EasyNmeaTest` class, which derives from |EasyNmea-api|, adding
the possibility of substituting the |EasyNmeaImpl-api| with another instance.
This enables the tests to implement a :class:`EasyNmeaImplMock`, which derives from |EasyNmeaImpl-api|,
mocking away the |EasyNmeaImpl::open-api|, |EasyNmeaImpl::is_open-api|, |EasyNmeaImpl::close-api|,
|EasyNmeaImpl::wait_for_data-api|, and |EasyNmeaImpl::take_next-api| functions.
This way, the tests can substitute the |EasyNmeaImpl-api| instance in :class:`EasyNmeaTest` with an instance
of :class:`EasyNmeaImplMock` on which expectations can be set, and then check whether |EasyNmea-api| behaves
as expected depending on the |EasyNmeaImpl-api| returned values.

.. uml:: ../../../uml/EasyNmeaTests.puml

.. contents::
    :depth: 1
    :local:

.. _unit_tests_easynmea_open:

open()
------

1. **openOk**: Check that |EasyNmea::open-api| passes the correct arguments to |EasyNmeaImpl::open-api|, and
   that it returns |ReturnCode::RETURN_CODE_OK-api| whenever |EasyNmeaImpl::open-api| does so.
2. **openError**: Check that |EasyNmea::open-api| passes the correct arguments to |EasyNmeaImpl::open-api|, and
   that it returns |ReturnCode::RETURN_CODE_ERROR-api| whenever |EasyNmeaImpl::open-api| does so.
3. **openIllegal**: Check that |EasyNmea::open-api| passes the correct arguments to |EasyNmeaImpl::open-api|, and
   that it returns |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api| whenever |EasyNmeaImpl::open-api| does so.

.. _unit_tests_easynmea_is_open:

is_open()
---------

1. **is_openOpened**: Check that |EasyNmea::is_open-api| returns ``true`` when a connection is opened.
2. **is_openClosed**: Check that |EasyNmea::is_open-api| returns ``false`` when a connection is closed.

.. _unit_tests_easynmea_close:

close()
-------

1. **closeOk**: Check that |EasyNmea::close-api| returns |ReturnCode::RETURN_CODE_OK-api| when an opened port is
   closed correctly.
2. **closeError**: Check that |EasyNmea::close-api| returns |ReturnCode::RETURN_CODE_ERROR-api| when an opened port
   cannot be closed correctly.
3. **closeIllegal**: Check that |EasyNmea::close-api| returns |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api| when
   attempting to close an already closed port.

.. _unit_tests_easynmea_take_next:

take_next()
-----------

1. **take_nextOk**: Check that |EasyNmea::take_next-api| calls to |EasyNmeaImpl::take_next-api| with the
   appropriate arguments, and that if returns |ReturnCode::RETURN_CODE_OK-api| whenever
   |EasyNmeaImpl::take_next-api| does so.
   Furthermore, check that the data output is the sample output by |EasyNmeaImpl::take_next-api|.
2. **take_nextNoData**: Check that |EasyNmea::take_next-api| calls to |EasyNmeaImpl::take_next-api| with the
   appropriate arguments, and that if returns |ReturnCode::RETURN_CODE_OK-api| whenever
   |EasyNmeaImpl::take_next-api| does so.
   Furthermore, check that the data output is equal to the input.

.. _unit_tests_easynmea_wait_for_data:

wait_for_data()
---------------

1. **wait_for_dataOk**: Check that |EasyNmeaImpl::wait_for_data-api| is called with the appropriate arguments, and
   that |EasyNmea::wait_for_data-api| returns |ReturnCode::RETURN_CODE_OK-api| whenever
   |EasyNmeaImpl::wait_for_data-api| does so.
2. **wait_for_dataTimeout**: Check that |EasyNmeaImpl::wait_for_data-api| is called with the appropriate arguments,
   and that |EasyNmea::wait_for_data-api| returns |ReturnCode::RETURN_CODE_TIMEOUT-api| whenever
   |EasyNmeaImpl::wait_for_data-api| does so.
3. **wait_for_dataTimeoutDefault**: The difference with **wait_for_dataTimeout** os that in this case,
   |EasyNmea::wait_for_data-api| is called leaving the timeout as default.
4. **wait_for_dataIllegal**: Check that |EasyNmeaImpl::wait_for_data-api| is called with the appropriate arguments,
   and that |EasyNmea::wait_for_data-api| returns |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api| whenever
   |EasyNmeaImpl::wait_for_data-api| does so.
5. **wait_for_dataError**: Check that |EasyNmeaImpl::wait_for_data-api| is called with the appropriate arguments,
   and that |EasyNmea::wait_for_data-api| returns |ReturnCode::RETURN_CODE_ERROR-api| whenever
   |EasyNmeaImpl::wait_for_data-api| does so.
