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

#include "xdg-shell-unstable-v6_karuta_server.h"
#include "instance.h"
#include "surface.h"
#include "log.h"

namespace karuta {

class XdgSurface : public protocol::ZxdgSurfaceV6, public Instance<XdgSurface> {
    friend class Instance<XdgSurface>;

private:
    Surface* surface_;
    XdgSurface(Surface* surface, Client& client, uint32_t version, uint32_t id);

public:
    void destroy(Client& client, ResourceRef& resource) override {
        debug("%s", __func__);
    }

    void get_toplevel(Client& client, ResourceRef& resource,
                      uint32_t id) override;

    void get_popup(Client& client, ResourceRef& resource, uint32_t id,
                   ResourceRef& parent, ResourceRef& positioner) override {
        debug("%s", __func__);
    }

    void set_window_geometry(Client& client, ResourceRef& resource, int32_t x,
                             int32_t y, int32_t width,
                             int32_t height) override {
        debug("%s", __func__);
    }

    void ack_configure(Client& client, ResourceRef& resource,
                       uint32_t serial) override {
        debug("%s", __func__);
    }
};

}  // karuta
