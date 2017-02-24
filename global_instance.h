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

#include <wayland-server.h>

#include "display.h"
#include "resource.h"
#include "client.h"

#include "create_instance.h"

namespace karuta {

template <typename T>
class GlobalInstance : public CreateInstance<T> {
protected:
    GlobalInstance(wl::Display& display) {
        wl_global_create(display.get_wl_display(), T::get_wl_interface(),
                         static_cast<int>(T::version), static_cast<void*>(this),
                         &global_bind);
    }

    virtual std::unique_ptr<wl::Resource> bind(wl::Client& client,
                                               uint32_t version, uint32_t id) {
        return this->create(client, version, id);
    }

private:
    static void global_bind(wl_client* client, void* data, uint32_t version,
                            uint32_t id) {
        auto self = static_cast<GlobalInstance*>(data);
        wl::Client c(client);
        self->bind(c, version, id);
    }
};

}  // karuta
