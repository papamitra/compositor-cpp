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
#include "surface.h"
#include "backend.h"

#include <functional>
#include <memory>
#include <cassert>
#include <dlfcn.h>

namespace karuta {
namespace wl {

Compositor::Compositor(wl::Display& display)
    : GlobalInstance(display), display_(display) {
}

void Compositor::init() {
    void* const module = dlopen("backend/x11/backend-x11.so", RTLD_NOW);
    auto* const create_backend = reinterpret_cast<create_backend_func_t>(dlsym(module, "karuta_create_backend"));
    assert(create_backend);

    Backend* backend = create_backend();
    backend->init();
}

void Compositor::create_surface(Client& client, Resource& resource,
                                uint32_t id) {
    Surface::create(client, resource.get_version(), id);
}

void Compositor::create_region(Client& client, Resource& resource,
                               uint32_t id) {
}

}  // wl
}  // karuta
