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

#include "xdg_surface.h"
#include "xdg_toplevel.h"
#include "array.h"
#include "resource_ref.h"

namespace karuta {

XdgSurface::XdgSurface(Surface* surface, Client& client, uint32_t version,
                       uint32_t id)
    : surface_(surface), Instance(client, version, id) {
}

void XdgSurface::get_toplevel(Client& client, ResourceRef& resource,
                              uint32_t id) {
    debug("%s", __func__);

    auto toplevel =
        XdgToplevel::create(this, client, resource.get_version(), id);

    Array<XdgToplevel::State> states;

    toplevel->send_configure(250, 250, states);
    send_configure(1 /* TODO */);
}

}  // karuta
