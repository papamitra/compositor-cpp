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

#include "wayland_karuta_server.h"
#include "instance.h"
#include "log.h"

#include <utility>

namespace karuta {

class BufferRefImpl;
class BufferRef;

class Buffer {
    friend class BufferRef;
    wl_listener destroy_listener_;
    ResourceRef resource_;
    int busy_count_ = 0;

public:
    static BufferRef from_resource(ResourceRef& resource);

    ~Buffer() {
        // TODO:
    }

private:
    Buffer(ResourceRef& resource)
        : resource_(resource) {}

    void destroy(Client& client, ResourceRef& resource) {
        debug("%s", __func__);
    }

    static void destroy_handler(wl_listener* listener, void* data);

    void destroy_ref(BufferRefImpl*) {
        assert(busy_count_ > 0);
        busy_count_--;
        if (busy_count_ == 0) {
            debug("buffer release");
            if (resource_)
                wl_resource_queue_event(resource_.get_wl_resource(),
                                        WL_BUFFER_RELEASE);
        }
    }

    void add_ref(BufferRefImpl*) { busy_count_++; }

public:
    Buffer()
        : resource_() {}  // for test

    ResourceRef resource() { return resource_; }
};

struct BufferRefImpl {
    Buffer* buffer_;
};

class BufferRef {
    BufferRefImpl* impl_;

public:
    BufferRef()
        : impl_(nullptr) {}

    explicit BufferRef(Buffer* buffer) {
        impl_ = new BufferRefImpl{buffer};
        if (impl_->buffer_) impl_->buffer_->add_ref(impl_);
    }

    BufferRef(const BufferRef& rhs) {
        impl_ = new BufferRefImpl{rhs.impl_ ? rhs.impl_->buffer_ : nullptr};
        if (impl_->buffer_) impl_->buffer_->add_ref(impl_);
    }

    BufferRef(BufferRef&& rhs)
        : impl_(nullptr) {
        swap(*this, rhs);
    }

    BufferRef& operator=(BufferRef& rhs) {
        BufferRef tmp(rhs);
        swap(*this, tmp);
    }

    BufferRef& operator=(BufferRef&& rhs) { swap(*this, rhs); }

    ~BufferRef() {
        if (!impl_) return;

        if (impl_->buffer_) impl_->buffer_->destroy_ref(impl_);
        delete impl_;
    }

    static void swap(BufferRef& lhs, BufferRef& rhs) {
        std::swap(lhs.impl_, rhs.impl_);
    }

    Buffer* get() {
        return impl_->buffer_;
    }

    operator bool() {
        if (!impl_) return false;
        return !!impl_->buffer_;
    }
};

}  // karuta
