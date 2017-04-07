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

#include "resource.h"
#include "impl_interface.h"
#include "log.h"

namespace karuta {

static void destroy_resource(struct wl_resource* resource) {
    debug("destroy_resource");
    ImplInterface* impl = static_cast<ImplInterface*>(wl_resource_get_user_data(resource));
    delete impl;
}

void Resource::set_implementation(ImplInterface& impl, bool delete_with_destroy) {
    if (delete_with_destroy)
        wl_resource_set_implementation(resource_, impl.get_interface(),
                                       &impl, destroy_resource);
    else
        wl_resource_set_implementation(resource_, impl.get_interface(),
                                       &impl, nullptr);
}

Resource::~Resource() {
    debug("delete resource");
    wl_resource_destroy(resource_);
    resource_ = nullptr;
}

}  // karuta
