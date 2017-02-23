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

#include "compositor.h"

#include "display.h"
#include "resource.h"
#include "client.h"

#include <functional>
#include <memory>

namespace karuta::wl {

Compositor::Compositor(wl::Display& display)
    : display_(display) {
    using namespace std::placeholders;

    display_.global_create(&wl_compositor_interface, 4,
                           std::bind(&Compositor::bind, this, _1, _2, _3));
}

void Compositor::create_surface(uint32_t id) {
}

void Compositor::create_region(uint32_t id) {
}

void Compositor::bind(wl::Client& client, uint32_t version, uint32_t id) {
    std::unique_ptr<wl::Resource> resource =
        client.resource_create(&wl_compositor_interface, version, id);

    resource->set_implementation(*this);
}

}  // karuta::wl
