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

namespace karuta {

class Surface : public protocol::WlSurface, public Instance<Surface> {
    friend class Instance<Surface>;

private:
    Surface(Client& client, uint32_t version, uint32_t id);

private:
    void destroy(Client& client, ResourceRef& resource) override {}

    void attach(Client& client, ResourceRef& resource, ResourceRef& buffer, int32_t x,
                int32_t y) override;

    void damage(Client& client, ResourceRef& resource, int32_t x, int32_t y,
                int32_t width, int32_t height) override {
        debug("%s", __func__);
    }

    void frame(Client& client, ResourceRef& resource, uint32_t callback) override;

    void set_opaque_region(Client& client, ResourceRef& resource,
        ResourceRef& region) override {}

    void set_input_region(Client& client, ResourceRef& resource,
        ResourceRef& region) override {}

    void commit(Client& client, ResourceRef& resource) override {}

    void set_buffer_transform(Client& client, ResourceRef& resource,
        int32_t transform) override {}

    void set_buffer_scale(Client& client, ResourceRef& resource,
        int32_t scale) override {}

    void damage_buffer(Client& client, ResourceRef& resource,
        int32_t x,
        int32_t y,
        int32_t width,
        int32_t height) override {}

};

}  // karuta
