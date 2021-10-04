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
"""Open a serial connection and send data specified in a file."""

import argparse
import logging
import os
import serial
import sys
import time


class InfoFilter(logging.Filter):
    """Class to filter logging message according to their level."""

    def filter(self, rec):
        """Filter all messages with level other than DEBUG or INFO."""
        return rec.levelno in (logging.DEBUG, logging.INFO)


if __name__ == '__main__':

    # Arguments
    parser = argparse.ArgumentParser(
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )
    parser.add_argument(
        '-p',
        '--serial_port',
        help='The serial port to use [Defaults: /dev/ttyACM0]',
        required=False,
        default='/dev/ttyACM0'
    )
    parser.add_argument(
        '-b',
        '--baudrate',
        help='The connection baud rate in bauds [Defaults: 9600]',
        required=False,
        default=9600
    )
    parser.add_argument(
        '-f',
        '--sentences_file',
        help='Filename of the sentences file',
        required=True,
    )
    parser.add_argument(
        '-t',
        '--line_period',
        help='Period in between sending lines',
        required=False,
        default=0
    )
    parser.add_argument(
        '-c',
        '--continuous',
        help='Send all lines in sentences_file in a continuous loop',
        action='store_true',
        required=False
    )
    parser.add_argument(
        '-d',
        '--debug',
        help='Output debug messages',
        required=False,
        action='store_true'
    )
    args = parser.parse_args()

    # Create a custom logger
    logger = logging.getLogger('send2serial')
    # Create handlers
    l_handler_1 = logging.StreamHandler(sys.stdout)
    if args.debug:
        l_handler_1.setLevel(logging.DEBUG)
    else:
        l_handler_1.setLevel(logging.INFO)
    l_handler_1.addFilter(InfoFilter())
    l_handler_2 = logging.StreamHandler(sys.stderr)
    l_handler_2.setLevel(logging.WARNING)
    # Create formatters and add it to handlers
    l_format = '[%(levelname)s] %(message)s'
    l_format = logging.Formatter(l_format)
    l_handler_1.setFormatter(l_format)
    l_handler_2.setFormatter(l_format)
    # Add handlers to the logger
    logger.addHandler(l_handler_1)
    logger.addHandler(l_handler_2)
    if args.debug:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.INFO)

    # Check that sentences file exists
    if not os.path.isfile(args.sentences_file):
        logger.error('Invalid file {}'.format(args.sentences_file))
        exit(1)

    # Get lines to send
    lines = []
    with open(args.sentences_file, 'r') as f:
        lines = f.readlines()

    # Open serial connection and send sentences
    with serial.Serial(port=args.serial_port, baudrate=args.baudrate) as ser:
        try:
            while True:
                for line in lines:
                    logger.debug('Sending {}'.format(line.rstrip()))
                    ser.write(line.encode())
                    time.sleep(args.line_period)
                if args.continuous is False:
                    break
        except Exception as e:
            logger.error('Exception caught: {}'.format(e))

    exit(0)
