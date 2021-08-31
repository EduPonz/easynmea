// Copyright (c) 2021 Eduardo Ponz Segrelles.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

/**
 * @file FixedSizeQueue.hpp
 */

#ifndef _GNSS_INTERFACE_FIXED_SIZE_QUEUE_HPP_
#define _GNSS_INTERFACE_FIXED_SIZE_QUEUE_HPP_

#include <queue>
#include <deque>
#include <iostream>

namespace eduponz {
namespace gnss_interface {

template <
    typename T,
    int max_size,
    typename Container = std::deque<T>>
class FixedSizeQueue : public std::queue<T, Container>
{
public:

    void push(
            const T& value)
    {
        if (this->size() >= max_size)
        {
            this->c.pop_front();
        }
        std::queue<T, Container>::push(value);
    }

    void clear()
    {
        while (!this->c.empty())
        {
            this->c.pop_front();
        }
    }

};

} // namespace gnss_interface
} // namespace eduponz

#endif //_GNSS_INTERFACE_FIXED_SIZE_QUEUE_HPP_
