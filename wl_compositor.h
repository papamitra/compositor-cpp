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

#include "global_instance.h"

namespace karuta {
namespace protocol {
class WlDisplay;
class WlClient;
}
}

namespace karuta {
namespace protocol {

class WlCompositor : public WlCompositorInterface,
                     public GlobalInstance<WlCompositor> {
    WlDisplay& display_;

public:
    WlCompositor(WlDisplay& display);

    void init();

    void create_surface(WlClient& client, WlResource& resource,
                        uint32_t id) override;

    void create_region(WlClient& client, WlResource& resource,
                       uint32_t id) override;

    WlDisplay& display() { return display_; }
};

}  // protocol
}  // karuta