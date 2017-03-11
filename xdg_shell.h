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

#include "global_instance.h"
#include "log.h"

namespace karuta {

class XdgShell : public protocol::ZxdgShellV6, public GlobalInstance<XdgShell> {
public:
    XdgShell(Display& dislplay);

private:
    void destroy(Client& client, Resource& resource) override {}

    void create_positioner(Client& client, Resource& resource,
                           uint32_t id) override {
        debug("%s\n", __func__);
    }

    void get_xdg_surface(Client& client, Resource& resource, uint32_t id,
                         Resource& surface) override {
        debug("%s", __func__);
    }

    void pong(Client& client, Resource& resource, uint32_t serial) override {
        debug("%s", __func__);
    }
};

}  // karuta
