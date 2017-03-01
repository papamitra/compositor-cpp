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

namespace karuta {
namespace protocol {

class WlSurface : public WlSurfaceInterface, public Instance<WlSurface> {
    friend class Instance<WlSurface>;

private:
    WlSurface(WlClient& client, uint32_t version, uint32_t id);

private:
    void destroy(WlClient& client, WlResource& resource) override {}

    void attach(WlClient& client, WlResource& resource, class protocol::WlBuffer* buffer,
                int32_t x, int32_t y) override {}

    void damage(WlClient& client, WlResource& resource, int32_t x, int32_t y,
                int32_t width, int32_t height) override {}

    void frame(WlClient& client, WlResource& resource, uint32_t callback) override;

    void set_opaque_region(WlClient& client, WlResource& resource,
                           class protocol::WlRegion* region) override {}

    void set_input_region(WlClient& client, WlResource& resource,
                          class protocol::WlRegion* region) override {}

    void commit(WlClient& client, WlResource& resource) override {}

    void set_buffer_transform(WlClient& client, WlResource& resource,
                              int32_t transform) override {}

    void set_buffer_scale(WlClient& client, WlResource& resource,
                          int32_t scale) override {}

    void damage_buffer(WlClient& client, WlResource& resource, int32_t x, int32_t y,
                       int32_t width, int32_t height) override {}
};

}  // protocol
}  // karuta
