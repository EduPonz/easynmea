# Copyright (c) 2021 Eduardo Ponz Segrelles.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
"""Application to run EasyNMEA system tests."""

import argparse
import logging
import os
import pathlib
import subprocess
import threading
import time
import yaml


class SystemTest():
    """Class to create, run, and evaluate EasyNMEA system tests."""

    def __init__(
        self,
        nmea_write_app,
        easynmea_app,
        name,
        send_port,
        receive_port,
        virtual_port_ttl,
        virtual_port_start,
        nmea_write_app_ttl,
        nmea_write_app_start,
        easynmea_ttl,
        easynmea_start,
        nmea_sentences_file,
        output_file,
        validation_file,
        debug=False
    ) -> None:
        """Initialize the test parameters."""
        self.nmea_write_app = nmea_write_app
        self.easynmea_app = easynmea_app
        self.name = name
        self.send_port = send_port
        self.receive_port = receive_port
        self.virtual_port_ttl = virtual_port_ttl
        self.virtual_port_start = virtual_port_start
        self.nmea_write_app_ttl = nmea_write_app_ttl
        self.nmea_write_app_start = nmea_write_app_start
        self.easynmea_ttl = easynmea_ttl
        self.easynmea_start = easynmea_start
        self.nmea_sentences_file = nmea_sentences_file
        self.output_file = output_file
        self.validation_file = validation_file
        self.virtual_ports_status = False
        self.send_status = False
        self.receive_status = False
        self.debug = debug

    def _virtual_ports_thread(self):
        """Create the virtual ports."""
        time.sleep(self.virtual_port_start)
        cmd = 'socat pty,rawer,link={} pty,rawer,link={}'.format(
            self.send_port,
            self.receive_port
        )
        logger.debug("Creating virtual ports. Command: '{}'".format(cmd))
        try:
            subprocess.run(
                cmd.split(),
                timeout=self.virtual_port_ttl
            )
            self.virtual_ports_status = True
        except subprocess.TimeoutExpired:
            self.virtual_ports_status = False

    def _send_thread(self):
        """Instantiate the NMEA write application (send2serial)."""
        time.sleep(self.nmea_write_app_start)
        cmd = 'python3 {} --sentences_file {} --serial_port {}{}'.format(
            self.nmea_write_app,
            self.nmea_sentences_file,
            self.send_port,
            ' --debug' if self.debug else ''
        )
        logger.debug(
            "Running NMEA write application. Command: '{}'".format(cmd)
        )
        try:
            send = subprocess.run(
                cmd,
                shell=True,
                capture_output=True,
                timeout=self.nmea_write_app_ttl
            )
            self.send_status = True
        except subprocess.TimeoutExpired:
            self.send_status = False
        finally:
            if send.stderr is not None and send.stderr.decode() != '':
                for line in send.stderr.decode().split('\n'):
                    logger.error('[SENT] {}'.format(line.rstrip()))
            if send.stdout is not None and send.stdout.decode() != '':
                for line in send.stdout.decode().split('\n'):
                    logger.debug('[SENT] {}'.format(line.rstrip()))

    def _receive_thread(self):
        """Instantiate the EasyNMEA receiving application."""
        time.sleep(self.easynmea_start)
        cmd = '{} --serial_port {} --output_file {}'.format(
            self.easynmea_app,
            self.receive_port,
            self.output_file
        )
        logger.debug(
            "Running EasyNMEA read application. Command: '{}'".format(
                cmd
            )
        )
        stdout = None
        stderr = None
        retcode = 0
        process = subprocess.Popen(
            cmd.split(),
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE
        )
        try:
            stdout, stderr = process.communicate(
                input=None,
                timeout=self.easynmea_ttl
            )
        except subprocess.TimeoutExpired as exc:
            process.terminate()
            stdout = exc.stdout
            stderr = exc.stderr
            process.wait()
        except Exception:
            process.kill()
        retcode = process.poll()

        if stderr is not None and stderr != '':
            for line in stderr.decode().split('\n'):
                logger.error('[RECEIVED] {}'.format(line.rstrip()))

        if stdout is not None and stdout != '':
            for line in stdout.decode().split('\n'):
                logger.debug('[RECEIVED] {}'.format(line.rstrip()))
        self.receive_status = True if retcode == 0 else False

    def run(self):
        """Run the test."""
        logger.info('Running test {}'.format(self.name))
        virtual_ports_thread = threading.Thread(
            target=self._virtual_ports_thread
        )
        send_thread = threading.Thread(target=self._send_thread)
        receive_thread = threading.Thread(target=self._receive_thread)

        virtual_ports_thread.start()

        while (not pathlib.Path(self.send_port).is_symlink() or
               not pathlib.Path(self.receive_port).is_symlink()):
            logger.debug('Waiting for ports to be opened')
            time.sleep(0.1)

        send_thread.start()
        receive_thread.start()

        virtual_ports_thread.join()
        send_thread.join()
        receive_thread.join()

        pathlib.Path(self.send_port).unlink(missing_ok=True)
        pathlib.Path(self.receive_port).unlink(missing_ok=True)

        status = self.receive_status
        if self.validation_file is not None:
            status, expected, actual = self._validate()

        logger.info(
            'Test {} status: {}'.format(
                self.name,
                'OK' if status is True else 'FAIL'
            )
        )

        if self.validation_file is not None:
            if status is False:
                logger.error(
                    'Expected output: \n{}'.format(
                        yaml.dump(expected, indent=4)
                    )
                )
                logger.error(
                    'Actual output: \n{}'.format(yaml.dump(actual, indent=4))
                )

        return status

    def _validate(self):
        """Validate test results."""
        if not os.path.isfile(self.validation_file):
            logger.error(
                'Validation file {} does not exist'.format(
                    self.validation_file
                )
            )
            return False

        if not os.path.isfile(self.output_file):
            logger.error(
                'Output file {} does not exist'.format(self.output_file)
            )
            return False

        validation = {}
        with open(self.validation_file, 'r') as f:
            validation = yaml.safe_load(f)

        output = {}
        with open(self.output_file, 'r') as f:
            output = yaml.safe_load(f)

        return validation == output, validation, output


if __name__ == '__main__':

    # Arguments
    parser = argparse.ArgumentParser(
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )
    parser.add_argument(
        '-l',
        '--list_tests',
        help='List all tests available in the configuration file',
        required=False,
        action='store_true'
    )
    parser.add_argument(
        '-d',
        '--debug',
        help='Output debug messages',
        required=False,
        action='store_true'
    )
    parser.add_argument(
        '-c',
        '--config',
        help='System tests configuration file',
        required=False,
        default='system_tests.yaml'
    )
    parser.add_argument(
        '-t',
        '--tests',
        nargs='+',
        help='List of tests to run separated by spaces',
        required=False,
        default=None
    )
    args = parser.parse_args()

    # Create a custom logger
    logger = logging.getLogger('system_tests')
    # Create handlers
    l_handler = logging.StreamHandler()
    # Create formatters and add it to handlers
    l_format = '[%(levelname)s] %(message)s'
    l_format = logging.Formatter(l_format)
    l_handler.setFormatter(l_format)
    # Add handlers to the logger
    logger.addHandler(l_handler)
    # Set log level
    if args.debug:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.INFO)

    # Check that configuration file exists
    if not os.path.isfile(args.config):
        logger.error('{} is not a file'.format(args.config))
        exit(1)

    # Load configuration file
    with open(args.config, 'r') as f:
        config = yaml.safe_load(f)

    # List available tests and exit (if specified)
    if args.list_tests:
        for test_config in config['system_tests']:
            print(
                '- {}: {}'.format(
                    test_config['name'],
                    test_config['description']
                )
            )
        exit(0)

    # Check that the requested test are available in the configuration file
    available_tests = [
        test_config['name'] for test_config in config['system_tests']
    ]
    if args.tests is not None:
        for test in args.tests:
            if test not in available_tests:
                logger.error(
                    "Test '{}' is not defined in {}".format(test, args.config)
                )
                exit(1)

    # Prepare tests to be run
    exit_code = 0
    tests_to_run = []
    for test_config in config['system_tests']:
        if args.tests is None or test_config['name'] in args.tests:
            tests_to_run.append(test_config)

    # Run the tests sequentially
    for test_config in tests_to_run:
        test = SystemTest(
            config['nmea_write_app'],
            config['easynmea_app'],
            test_config['name'],
            test_config['send_port'],
            test_config['receive_port'],
            test_config['virtual_port_ttl'],
            (
                test_config['virtual_port_start']
                if 'virtual_port_start' in test_config
                else 0
            ),
            test_config['nmea_write_app_ttl'],
            (
                test_config['nmea_write_app_start']
                if 'nmea_write_app_start' in test_config
                else 0
            ),
            test_config['easynmea_app_ttl'],
            (
                test_config['easynmea_app_start']
                if 'easynmea_app_start' in test_config
                else 0
            ),
            test_config['nmea_sentences_file'],
            test_config['output_file'],
            (
                test_config['validation_file']
                if 'validation_file' in test_config
                else None
            ),
            debug=args.debug
        )
        exit_code += not test.run()

    exit(exit_code)
