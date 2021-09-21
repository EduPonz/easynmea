.. include:: ../../include/aliases.rst

.. _unit_tests_gnssinterfaceimpl:

GnssInterfaceImpl Unit Tests
============================

As documented in :ref:`dev_docs_libs_arch_impl`, |GnssInterfaceImpl-api| provides with the implementation for the
|GnssInterface-api| public API, namely opening and closing the serial port, waiting until data of one or more NMEA 0183
types has been received, checking whether the serial port connection is opened, and taking the next unread sample of a
given NMEA 0183 type.

The |GnssInterfaceImpl-api| tests use the :class:`GnssInterfaceImplTest` class, which derives from
|GnssInterfaceImpl-api|, adding the possibility of substituting the |SerialInterface-api| with another instance.
This enables the tests to implement a :class:`SerialInterfaceMock`, which derives from |SerialInterface-api|, mocking
away the |SerialInterface::open-api|, |SerialInterface::is_open-api|, |SerialInterface::close-api|, and
|SerialInterface::read_line-api| functions.
This way, the tests can substitute the |SerialInterface-api| instance in :class:`GnssInterfaceImplTest` with an instance
of :class:`SerialInterfaceMock` on which expectations can be set, and then check whether |GnssInterfaceImpl-api|
behaves as expected depending on the |SerialInterface-api| returned values.

.. uml:: ../../../uml/GnssInterfaceImplTests.puml

.. contents::
    :depth: 1
    :local:

.. _unit_tests_gnssinterfaceimpl_open:

open()
------

1. **openSuccess**: Opens a not previously opened |GnssInterfaceImpl-api|.
   The return is expected to be |ReturnCode::RETURN_CODE_OK-api|.
2. **openOpened**: Attempts to open an already opened |GnssInterfaceImpl-api|.
   This is simulated by forcing |SerialInterface::is_open-api| to return ``true``.
   The return is expected to be |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api|.
3. **openWrongPort**: Attempts to open a |GnssInterfaceImpl-api| on an invalid port.
   This is simulated by forcing |SerialInterface::open-api| to return ``false``.
   The return is expected to be |ReturnCode::RETURN_CODE_ERROR-api|.

.. _unit_tests_gnssinterfaceimpl_is_open:

is_open()
---------

1. **is_openOpened**: Check that whenever |SerialInterface::is_open-api| returns ``true``,
   |GnssInterfaceImpl::is_open-api| also returns ``true``.
2. **is_openClosed**: Check that whenever |SerialInterface::is_open-api| returns ``false``,
   |GnssInterfaceImpl::is_open-api| also returns ``false``.
   Furthermore, this test also checks that |GnssInterfaceImpl::is_open-api| returns ``false`` whenever the underlying
   pointer to |SerialInterface-api| is ``nullptr``.

.. _unit_tests_gnssinterfaceimpl_close:

close()
-------

1. **closeSuccess**: Check that whenever |SerialInterface-api| reports that a port is opened at first, and then return
   ``true`` on the call to |SerialInterface::close-api|, then |GnssInterfaceImpl::close-api| returns
   |ReturnCode::RETURN_CODE_OK-api|.
2. **closeError**: Check that whenever |SerialInterface-api| reports that a port is opened at first, and then return
   ``false`` on the call to |SerialInterface::close-api|, then |GnssInterfaceImpl::close-api| returns
   |ReturnCode::RETURN_CODE_ERROR-api|.
3. **closeClosed**: Check that calling |GnssInterfaceImpl::close-api| on a non-opened |GnssInterfaceImpl-api| returns
   |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api|.

.. _unit_tests_gnssinterfaceimpl_wait_for_data:

wait_for_data()
---------------

1. **wait_for_dataData**: Check that |GnssInterfaceImpl::wait_for_data-api| returns |ReturnCode::RETURN_CODE_OK-api|
   when a sentence which type specified in the |NMEA0183DataKindMask-api| mask is received.
   Furthermore, check that the output mask has the corresponding bit correctly set.
2. **wait_for_dataClosed**: Check that |GnssInterfaceImpl::wait_for_data-api| returns
   |ReturnCode::RETURN_CODE_ILLEGAL_OPERATION-api| when called on a closed |GnssInterfaceImpl-api|.
3. **wait_for_dataDataEmptyMask**: Check that |GnssInterfaceImpl::wait_for_data-api| will return
   |ReturnCode::RETURN_CODE_NO_DATA-api| after timing out when an empty |NMEA0183DataKindMask-api| is passed, even when
   data from any of the supported types has been received.
   It also checks that the output |NMEA0183DataKindMask-api| is set to ``none``.
4. **wait_for_dataError**: Check that whenever |SerialInterface::read_line-api| returns false, the call to
   |GnssInterfaceImpl::wait_for_data-api| unblocks and returns |ReturnCode::RETURN_CODE_ERROR-api|.
   It also checks that the output |NMEA0183DataKindMask-api| is set to ``none``.

.. _unit_tests_gnssinterfaceimpl_take_next:

take_next()
-----------

1. **take_next**: Check that whenever |GnssInterfaceImpl::wait_for_data-api| returns |ReturnCode::RETURN_CODE_OK-api|,
   then, data can be taken with |GnssInterfaceImpl::take_next-api|, which returns |ReturnCode::RETURN_CODE_OK-api|.
   Furthermore, it tests that other NMEA 0183 valid sentences are not returned nor reported to be have been received,
   and that incomplete GPGGA sentences are not returned nor reported either.

.. _unit_tests_gnssinterfaceimpl_destructor:

~GnssInterfaceImpl()
--------------------

1. **destroyNoClose**: Checks that letting an opened |GnssInterfaceImpl-api| instance go out of scope without calling
   |GnssInterfaceImpl::close-api| is alright.
