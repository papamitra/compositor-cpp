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

#include "server-protocol.h"

#include "instance.h"

namespace karuta {
namespace wl {

class Surface : public SurfaceInterface, public Instance<Surface> {
    friend class Instance<Surface>;

private:
    Surface(Client& client, uint32_t version, uint32_t id);

private:
    void destroy(Client& client, Resource& resource) override {}

    void attach(Client& client, Resource& resource, class wl::Buffer* buffer,
                int32_t x, int32_t y) override {}

    void damage(Client& client, Resource& resource, int32_t x, int32_t y,
                int32_t width, int32_t height) override {}

    void frame(Client& client, Resource& resource, uint32_t callback) override;

    void set_opaque_region(Client& client, Resource& resource,
                           class wl::Region* region) override {}

    void set_input_region(Client& client, Resource& resource,
                          class wl::Region* region) override {}

    void commit(Client& client, Resource& resource) override {}

    void set_buffer_transform(Client& client, Resource& resource,
                              int32_t transform) override {}

    void set_buffer_scale(Client& client, Resource& resource,
                          int32_t scale) override {}

    void damage_buffer(Client& client, Resource& resource, int32_t x, int32_t y,
                       int32_t width, int32_t height) override {}
};

}  // wl
}  // karuta