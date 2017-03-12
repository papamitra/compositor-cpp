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

#include "buffer.h"
#include "utils.h"

namespace karuta {

Buffer* Buffer::buffer_from_resource(ResourceRef& resource) {
    wl_listener* listener = wl_resource_get_destroy_listener(resource.get_wl_resource(), Buffer::destroy_handler);

    if (listener)
        return karuta_container_of(listener, &Buffer::destroy_listener_);

    auto buffer = new Buffer{resource};
    buffer->destroy_listener_.notify = Buffer::destroy_handler;
    wl_resource_add_destroy_listener(resource.get_wl_resource(),
                                     &buffer->destroy_listener_);
    return buffer;
}

void Buffer::destroy_handler(wl_listener* listener, void *data) {

}

}  // karuta
