.. include:: ../../include/aliases.rst

.. _unit_tests_serialinterface:

SerialInterface Unit Tests
==========================

As documented in :ref:`dev_docs_libs_arch_serial`, |SerialInterface-api| provides functions to open, close, and read
from serial ports using Asio.
The |SerialInterface-api| tests use a :class:`SerialInterfaceTest` class which derives from |SerialInterface-api|
specialized in :class:`SerialPortMock`, which mocks :class:`asio::serial_port`.

.. uml:: ../../../uml/SerialInterfaceTests.puml

1. Since :class:`SerialInterfaceTest` creates its :class:`SerialPortMock` in the constructor, no expectations can be set
   to that object.
   For this reason, :class:`SerialInterfaceTest` provides a :func:`set_serial_port` public member function that can be
   used to substitute the :class:`SerialPortMock` instance with one on which expectations have been set.
2. To be able to construct this :class:`SerialPortMock`, a getter :func:`io_service` is also provided.
3. Some tests need to mock :func:`SerialPort::read_some` (:func:`asio::serial_port::read_some`) so that
   |SerialInterface::read_line-api| returns a specific :class:`std::string`.
   To that end, |SerialInterface-api| wraps the call to :func:`SerialPort::read_some` with a :func:`read_char`, which
   |SerialInterface::read_line-api| calls to perform the actual read from the port.
   Since for unit testing purposes :class:`SerialPortMock` is used instead of :class:`asio::serial_port`, a mock
   :func:`SerialPortMock::read_some` would be needed.
   However, due to the function's signature, it is not possible to set expectations on the read characters.
   This has led to :class:`SerialInterfaceTest` overriding :func:`SerialInterface::read_char` with an overload that
   either simply calls to the :func:`SerialInterface::read_char` implementation, or returns a character from a
   string.
   To do this, :class:`SerialInterfaceTest` provides a :func:`set_msg` function that is used to set the
   line that :class:`read_line` will read.
   To enable :func:`SerialInterfaceTest::read_char` to read characters from the set message instead of using
   :func:`read_some`, a :func:`use_parent_read_char` is provided.
   By default, :func:`SerialInterfaceTest::read_char` will call :func:`SerialInterface::read_char` (which calls
   :func:`read_some`), however, if the ``use_parent_read_char_`` flag is set (calling
   :class:`use_parent_read_char(false)`), then :func:`SerialInterfaceTest::read_char` will read the characters of the
   set message one at a time (simulating reading characters one by one from the serial port).

.. contents::
    :depth: 1
    :local:

.. _unit_tests_serialinterface_open:

open()
------

1. **openSuccess**: Opens a not previously opened serial port with a valid port and baudrate. The return is expected to
   be ``true``
2. **openOpened**: Attempts to open an already opened port. The return is expected to be ``false``.
3. **openWrongPort**: Attempts to open a port on an invalid port. The return is expected to be ``false``.
4. **openWrongBaudrate**: Attempts to set a non valid baudrate to the serial port. The return is expected to be
   ``false``.

.. _unit_tests_serialinterface_is_open:

is_open()
---------

1. **is_openOpened**: Checks whether |SerialInterface::is_open-api| returns ``true`` for an open port.
2. **is_openClosed**: Checks whether |SerialInterface::is_open-api| returns ``false`` for an closed port.

.. _unit_tests_serialinterface_close:

close()
-------

1. **closeSuccess**: Closes an already opened port. The return is expected to be ``true``.
2. **closeClosed**: Closes an already closed port. The return is expected to be ``true``.
3. **closeAsioError**: Attempts to close an open port that Asio cannot close. The return is expected to be ``false``.

.. _unit_tests_serialinterface_read_line:

read_line()
-----------

1. **read_lineSuccess**: Checks that lines ending in ``\n`` or ``\r\n`` are returned correctly. The return is expected
   to be ``true``.
   This test is performed on an opened serial port.
   Furthermore, the function should be called with an empty string, as well as with a non-empty one.
   Both cases should output just the read line without any characters that it had on calling
   |SerialInterface::read_line-api|.
2. **read_lineClosed**: Checks that calling |SerialInterface::read_line-api| on a closed port returns ``false``.
3. **read_lineReadError**: Simulates that `asio::serial_port::read_some()` returns an error and checks that in the case,
   the |SerialInterface::read_line-api| return is ``false``.
   This test covers the case when `asio::serial_port::close()` is called while blocked on
   `asio::serial_port::read_some()`, since that breaks the block, making `asio::serial_port::read_some()` return with a
   not OK `asio::error_code`.
