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

/**
 * @class FixedSizeQueue
 *
 * This template class provides a fixed size container for containers which abide to the std::deque
 * API, mainly having the member functions: \c push(), \c size(), \c pop_front(), and \c empty().
 *
 * \c FixedSizeQueue provides a \c push() method that automatically deletes the front element of the
 * queue before adding the new one if needed, i.e. if the queue had already reached the maximum
 * number of elements. Furthermore, it provides a \c clear() function to eliminate all the elements
 * in the \c FixedSizeQueue.
 *
 * @tparam T: The type of the elements of the \c FixedSizeQueue.
 * @tparam max_size: The maximum number of elements that the \c FixedSizeQueue can contain.
 * @tparam Container: The underlying container used. Defaults to std::deque<T>>
 */
template <
    typename T,
    int max_size,
    typename Container = std::deque<T>>
class FixedSizeQueue : public std::queue<T, Container>
{
public:

    /**
     * Push a new element into the \c FixedSizeQueue.
     *
     * Push a new element into the \c FixedSizeQueue, making space for it if necessary, meaning that
     * if the number of elements in the \c FixedSizeQueue is already max_size, the first element
     * will be removed before adding the new one.
     *
     * @param value: A constant reference to the element to be pushed. Mind that the type of the
     *               element is \c T.
     */
    void push(
            const T& value)
    {
        if (this->size() >= max_size)
        {
            this->c.pop_front();
        }
        std::queue<T, Container>::push(value);
    }

    /**
     * Remove all the elements from the \c FixedSizeQueue. After calling \c clear() the return of
     * \c size() is 0.
     */
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
