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

FROM ubuntu:latest

# Avoid interactuation with installation of some package that needs the locale.
ENV TZ=Europe/Madrid
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install tools and dependecies
RUN apt -y update && apt -y install \
    cmake \
    g++ \
    wget \
    git \
    python3-pip \
    libasio-dev \
    python3-venv \
    doxygen \
    libgtest-dev \
    libgmock-dev \
    vim

# Build GTest and GMock
RUN cd /usr/src/gtest && \
    cmake . && \
    make -j $(nproc) -C /usr/src/gtest && \
    ln -s /usr/src/gtest/libgtest.a /usr/lib/libgtest.a && \
    ln -s /usr/src/gtest/libgtest_main.a /usr/lib/libgtest_main.a && \
    cd /usr/src/googletest/googlemock && \
    cmake . && \
    make -j $(nproc) -C /usr/src/googletest/googlemock && \
    ln -s /usr/src/googletest/googlemock/libgmock.a /usr/lib/libgmock.a

# Download and build library with examples, documentation, and tests
RUN cd ~ && \
    git clone https://github.com/EduPonz/gnss_interface.git && \
    cd gnss_interface && \
    pip3 install -r docs/requirements.txt && \
    mkdir build && cd build && \
    cmake .. -DBUILD_EXAMPLES=ON -DBUILD_DOCUMENTATION=ON -DBUILD_TESTS=ON && \
    cmake --build .

RUN apt autoclean

WORKDIR /root
