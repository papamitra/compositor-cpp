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

#include "log.h"

#include <wayland-util.h>
#include <utility>
#include <functional>
#include <cassert>

namespace karuta {

class ArrayRef {
    wl_array* array_;

    ArrayRef() = delete;

public:
    ArrayRef(wl_array* array) : array_(array) {}

    wl_array* get_wl_array() { return array_; }

    template <typename T>
    bool add(const T& val) {
        assert(array_);
        check_alloc_size(sizeof(T));

        auto p = static_cast<T*>(wl_array_add(array_, sizeof(val)));
        if (!p) {
            return false;
        }
        *p = val;
        return true;
    }

    template <typename T>
    void for_each(std::function<void(const T&)> func) {
        assert(array_);
        check_alloc_size(sizeof(T));

        T pos;
        wl_array_for_each(pos, array_) { func(*pos); }
    }

private:
    void check_alloc_size(size_t size) {
        if (array_->alloc % size != 0) {
            warn("ArrayRef.add size mismatch: add=%zu, allocated=%zu\n", size,
                 array_->alloc);
        }
    }
};

template <typename T>
class Array {
    wl_array array_;
    ArrayRef ref_;

    Array(const Array&) = delete;

public:
    Array() : array_(), ref_(&array_) { wl_array_init(&array_); }

    Array(Array&& rhs) : array_(), ref_(&array_) {
        std::swap(array_, rhs.array_);
    }

    ~Array() { wl_array_release(&array_); }

    bool add(const T& val) { return ref_.add(val); }

    T* begin() { return static_cast<T*>(array_.data); }

    T* end() {
        return reinterpret_cast<T*>(static_cast<char*>(array_.data) +
                                    static_cast<ptrdiff_t>(array_.size));
    }

    operator ArrayRef&() { return ref_; }
};

}  // karuta
