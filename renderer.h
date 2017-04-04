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

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

namespace karuta {

class Compositor;
class Surface;

class Renderer {
    GLuint program_;
    GLuint position_;
    GLuint texcoord_;
    GLuint texture_index_;
    GLuint texture_[1];

    EGLDisplay egl_display_;
	EGLConfig egl_config_;

    EGLSurface surface_;
    EGLContext context_;

    EGLNativeWindowType native_window_;

    Compositor& compositor_;
public:
    Renderer(Compositor& compositor);
    ~Renderer();

    static Renderer& get_instance();

    bool egl_init(EGLDisplay egl_display, EGLNativeWindowType window);

    bool create_surface();

    void draw(Surface& surface);

};

}  // karuta
