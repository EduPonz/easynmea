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
 * @file visibility.hpp
 */

#ifndef _EASYNMEA_VISIBILITY_HPP_
#define _EASYNMEA_VISIBILITY_HPP_

/*
 * This logic was borrowed from the examples on the gcc wiki: https://gcc.gnu.org/wiki/Visibility
 */
#if defined _WIN32 || defined __CYGWIN__
    #ifdef __GNUC__
        #define EASYNMEA_EXPORT __attribute__ ((dllexport))
        #define EASYNMEA_IMPORT __attribute__ ((dllimport))
    #else
        #define EASYNMEA_EXPORT __declspec(dllexport)
        #define EASYNMEA_IMPORT __declspec(dllimport)
    #endif
    #ifdef EASYNMEA_BUILDING_LIBRARY
        #define EASYNMEA_PUBLIC EASYNMEA_EXPORT
    #else
        #define EASYNMEA_PUBLIC EASYNMEA_IMPORT
    #endif
    #define EASYNMEA_PUBLIC_TYPE EASYNMEA_PUBLIC
    #define EASYNMEA_LOCAL
#else
    #define EASYNMEA_EXPORT __attribute__ ((visibility("default")))
    #define EASYNMEA_IMPORT
    #if __GNUC__ >= 4
        #define EASYNMEA_PUBLIC __attribute__ ((visibility("default")))
        #define EASYNMEA_LOCAL  __attribute__ ((visibility("hidden")))
    #else
        #define EASYNMEA_PUBLIC
        #define EASYNMEA_LOCAL
    #endif
    #define EASYNMEA_PUBLIC_TYPE
#endif

#endif // _EASYNMEA_VISIBILITY_HPP_
