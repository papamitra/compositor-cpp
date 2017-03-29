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

#include "backend-x11.h"
#include "compositor.h"
#include "display.h"
#include "log.h"
#include "renderer.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <cstdlib>
#include <cassert>

namespace karuta {

BackendX11::BackendX11(Compositor *compositor)
    : egl_display_(EGL_NO_DISPLAY)
    , compositor_(compositor) {
}

bool BackendX11::init() {
    // FIXME: error handling.

    display_ = XOpenDisplay(NULL);

    conn_ = XGetXCBConnection(display_);
    if (!conn_) {
        error("xcb_connect() failed");
        return false;
    }

    screen_ = xcb_setup_roots_iterator(xcb_get_setup(conn_)).data;

    window_ = xcb_generate_id(conn_);

    xcb_create_window(
        conn_, XCB_COPY_FROM_PARENT, window_, screen_->root, 0, 0,  // x, y
        1024, 768,  // width, height
        10,         // border width
        XCB_WINDOW_CLASS_INPUT_OUTPUT, screen_->root_visual, 0, NULL);

    xcb_map_window(conn_, window_);

    xcb_flush(conn_);

    if (!egl_init()) {
        return false;
    }

    return true;
}

bool BackendX11::egl_init() {
    auto get_platform_display =
        reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(
            eglGetProcAddress("eglGetPlatformDisplayEXT"));
    if (get_platform_display) {
        egl_display_ =
            get_platform_display(EGL_PLATFORM_X11_KHR, display_, NULL);
    }

    if (egl_display_ == EGL_NO_DISPLAY) {
        error("failed to create display\n");
        return false;
    }

    return compositor_->renderer().egl_init(egl_display_, window_);
}

}  // karuta

extern "C" {
void *karuta_create_backend(void *compositor) {
    return new karuta::BackendX11(
        static_cast<karuta::Compositor *>(compositor));
}
}
