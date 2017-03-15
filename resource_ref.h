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

#include <wayland-server.h>
#include <cassert>

namespace karuta {

class ResourceRef {
    wl_resource* resource_;

public:
    ResourceRef(wl_resource* resource) : resource_(resource) {}
    ResourceRef(const ResourceRef& rhs) : resource_(rhs.resource_) {}

    wl_resource* get_wl_resource() { return resource_; }

    uint32_t get_version() const { return wl_resource_get_version(resource_); }

    operator bool() { return resource_; }

    template<typename T>
    T* user_data_to_object() {
        assert(resource_);
        return static_cast<T*>(wl_resource_get_user_data(resource_));
    }
};

}  // karuta
