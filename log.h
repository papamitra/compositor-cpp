/*
 * Copyright © 2017 papamitra
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <cstdio>
#include <cstdarg>

namespace karuta {

__attribute__ ((format(printf, 1, 2)))
static int debug(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("[Debug  ] ");
    auto ret = vfprintf(stderr, fmt, ap);
    va_end(ap);
    return ret;
}

__attribute__ ((format(printf, 1, 2)))
static int error(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("[Error  ] ");
    auto ret = vfprintf(stderr, fmt, ap);
    va_end(ap);
    return ret;
}

__attribute__ ((format(printf, 1, 2)))
static int warn(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("[Warning] ");
    auto ret = vfprintf(stderr, fmt, ap);
    va_end(ap);
    return ret;
}

}  // karuta
