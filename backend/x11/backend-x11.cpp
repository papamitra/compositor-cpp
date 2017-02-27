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
#include "log.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <cstdlib>

namespace karuta {

BackendX11::BackendX11()
    : egl_display_(EGL_NO_DISPLAY) {
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

    EGLint major, minor;
    if (!eglInitialize(egl_display_, &major, &minor)) {
        error("faile to initialize display\n");
        return false;
    }

    static const EGLint opaque_attribs[] = {
        EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,      //
        EGL_RED_SIZE,        1,                   //
        EGL_GREEN_SIZE,      1,                   //
        EGL_BLUE_SIZE,       1,                   //
        EGL_ALPHA_SIZE,      0,                   //
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,  //
        EGL_NONE};

    if (!egl_choose_config(opaque_attribs, NULL, 0, &egl_config_)) {
        error("faile to choose EGL config\n");
        return false;
    }

    return true;
}

static int match_config_to_visual(EGLDisplay egl_display, EGLint visual_id,
                                  EGLConfig *configs, int count) {
    int i;

    for (i = 0; i < count; ++i) {
        EGLint id;

        if (!eglGetConfigAttrib(egl_display, configs[i], EGL_NATIVE_VISUAL_ID,
                                &id))
            continue;

        if (id == visual_id) return i;
    }

    return -1;
}

bool BackendX11::egl_choose_config(const EGLint *attribs,
                                   const EGLint *visual_id, const int n_ids,
                                   EGLConfig *config_out) {
    EGLint count = 0;
    EGLint matched = 0;
    EGLConfig *configs;
    int i, config_index = -1;

    if (!eglGetConfigs(egl_display_, NULL, 0, &count) || count < 1) {
        error("No EGL configs to choose from.\n");
        return false;
    }

    configs = new EGLConfig[count]();
    if (!configs) return false;

    if (!eglChooseConfig(egl_display_, attribs, configs, count, &matched) ||
        !matched) {
        error("No EGL configs with appropriate attributes.\n");
        goto out;
    }

    if (!visual_id || n_ids == 0) config_index = 0;

    for (i = 0; config_index == -1 && i < n_ids; i++)
        config_index = match_config_to_visual(egl_display_, visual_id[i],
                                              configs, matched);

    if (config_index != -1) *config_out = configs[config_index];

out:
    delete[] configs;
    if (config_index == -1) return false;

    if (i > 1)
        debug(
            "Unable to use first choice EGL config with id"
            " 0x%x, succeeded with alternate id 0x%x.\n",
            visual_id[0], visual_id[i - 1]);
    return true;
}

}  // karuta

extern "C" {
void *karuta_create_backend() {
    return new karuta::BackendX11();
}
}
