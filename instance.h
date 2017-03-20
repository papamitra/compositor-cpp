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

#include "client.h"
#include "resource.h"
#include "resource_ref.h"

#include <functional>
#include <memory>

namespace karuta {

class InstanceBase {
public:
    virtual operator ResourceRef() = 0;
};

template <typename T>
class Instance : public InstanceBase {
    std::unique_ptr<Resource> res_;

protected:
    Instance(Client& client, uint32_t version, uint32_t id) {
        res_ = client.resource_create(T::get_wl_interface(), version, id);
        res_->set_implementation(*static_cast<T*>(this));
    }

public:
    template <typename... Args>
    static T* create(Args&&... args) {
        return new T(std::forward<Args>(args)...);
    }

    operator ResourceRef() override { return ResourceRef(*res_.get()); }
};

}  // karuta
