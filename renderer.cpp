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

#include "renderer.h"
#include "compositor.h"
#include "surface.h"
#include "log.h"

#include <cstdlib>
#include <cassert>
#include <string>

namespace {

const GLchar vertex_source[] = R"(
    attribute vec4 position;
    attribute vec2 texcoord;
    varying vec2 texcoordVarying;
    void main() {
        gl_Position = position;
        texcoordVarying = texcoord;
    }
)";

const GLchar fragment_source[] = R"(
    precision mediump float;
    varying vec2 texcoordVarying;
    uniform sampler2D texture;
    void main() {
        gl_FragColor = texture2D(texture, texcoordVarying);
    }
)";

const float vertices[] = {
    -1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
     1.0f, -1.0f, 0.0f};

const float texcoords[] = {
    0.0f, 0.0f,
    0.0f, 1.0f,
    1.0f, 0.0f,
    1.0f, 1.0f};

GLuint load_shader(GLenum shader_type, const char *source) {
    GLuint shader = glCreateShader(shader_type);
    karuta::debug("shader = %d", shader);
    if (shader) {
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
            if (length) {
                std::string log(static_cast<size_t>(length), ' ');
                glGetShaderInfoLog(shader, length, nullptr, &log[0]);
                karuta::warn("%s", log.c_str());

                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint create_program() {
    GLuint vshader = load_shader(GL_VERTEX_SHADER, vertex_source);
    assert(vshader);

    GLuint fshader = load_shader(GL_FRAGMENT_SHADER, fragment_source);
    assert(fshader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vshader);
    glAttachShader(program, fshader);
    glLinkProgram(program);
    GLint link_status = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);
    if (link_status != GL_TRUE) {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        if (length) {
            std::string log(static_cast<size_t>(length), ' ');
            glGetProgramInfoLog(program, length, nullptr, &log[0]);
        }
        glDeleteProgram(program);
        std::abort();
    }

    return program;
}

PFNEGLCREATEIMAGEKHRPROC create_image;
PFNEGLQUERYWAYLANDBUFFERWL query_buffer;

}

namespace karuta {

bool Renderer::egl_init(EGLDisplay egl_display, EGLNativeWindowType window) {
    egl_display_ = egl_display;
    native_window_ = window;

    EGLint major, minor;
    if (!eglInitialize(egl_display_, &major, &minor)) {
        error("faile to initialize display\n");
        return false;
    }

    auto bind_display = reinterpret_cast<PFNEGLBINDWAYLANDDISPLAYWL>(
        eglGetProcAddress("eglBindWaylandDisplayWL"));
    assert(bind_display);

    auto wl_disp = compositor_.display().get_wl_display();
    bind_display(egl_display_, wl_disp);

    create_image = reinterpret_cast<PFNEGLCREATEIMAGEKHRPROC>(eglGetProcAddress("eglCreateImageKHR"));
    query_buffer = reinterpret_cast<PFNEGLQUERYWAYLANDBUFFERWL>(eglGetProcAddress("eglQueryWaylandBufferWL"));

// TODO:
    create_surface();

    program_ = create_program();

    glUseProgram(program_);
    glActiveTexture(GL_TEXTURE0);

    position_ = glGetAttribLocation(program_, "position");
    warn("position_ = %d", position_);
    glEnableVertexAttribArray(position_);
    warn("texcoord_ = %d", texcoord_);
    texcoord_ = glGetAttribLocation(program_, "texcoord");
    glEnableVertexAttribArray(texcoord_);

    texture_index_ = glGetUniformLocation(program_, "texture");
    warn("texture_index_ = %d", texture_index_);
    glGenTextures(1, texture_);
    glBindTexture(GL_TEXTURE_2D, texture_[0]);

    return true;
}

Renderer::Renderer(Compositor &compositor)
    : compositor_(compositor) {
}

Renderer::~Renderer() {
    glDeleteTextures(1, texture_);
    glDeleteProgram(program_);
}

void Renderer::load_image() {
    static const unsigned char buffer[] = {
        0xff, 0xff, 0xff,
        0x00, 0x00, 0xff,
        0x00, 0xff, 0x00,
        0xff, 0x00, 0x00,
    };

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2 /* width */ , 2 /* height */,
                    0, GL_RGB, GL_UNSIGNED_BYTE, buffer);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Renderer::draw(Surface& surface) {

    BufferRef buffer = surface.buffer();
    if (!buffer) {
        warn("buffer is null");
        return;
    }

    ResourceRef res = buffer.get()->resource();

    int32_t width;
    query_buffer(egl_display_, res.get_wl_resource(),
                 EGL_WIDTH, &width);

    warn("Renderer::draw width=%d", width);

    load_image();

    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, texture_[0]);
    glVertexAttribPointer(texcoord_, 2, GL_FLOAT, GL_FALSE, 0, texcoords);
    glVertexAttribPointer(position_, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glUniform1i(texture_index_, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    eglSwapBuffers(egl_display_, surface_);
}

bool Renderer::create_surface() {
    const EGLint attr[] = {
        EGL_BLUE_SIZE,       8,
        EGL_GREEN_SIZE,      8,
        EGL_RED_SIZE,        8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE};

    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(egl_display_, attr, &config, 1, &numConfigs)) {
        warn("##### EGL Choose Config Error! #####");
        return false;
    }

    if (numConfigs != 1) {
        warn("##### EGL Num Config Error! #####");
        return false;
    }

    EGLint format;
    eglGetConfigAttrib(egl_display_, config, EGL_NATIVE_VISUAL_ID, &format);
    surface_ = eglCreateWindowSurface(egl_display_, config,
                                            native_window_, nullptr);
    if (surface_ == EGL_NO_SURFACE) {
        warn("##### EGL NoSerufase Error! #####");
        return false;
    }

    EGLint ctxattr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    context_ =
        eglCreateContext(egl_display_, config, EGL_NO_CONTEXT, ctxattr);
    if (context_ == EGL_NO_CONTEXT) {
        warn("##### EGL Context Error! #####");
        return false;
    }
    if (!eglMakeCurrent(egl_display_, surface_, surface_,
                        context_)) {
        warn("##### EGL Mark Current Error! #####");
        return false;
    }
    EGLint w, h;
    eglQuerySurface(egl_display_, surface_, EGL_WIDTH, &w);
    eglQuerySurface(egl_display_, surface_, EGL_HEIGHT, &h);
    glViewport(0, 0, w, h);

    return true;
}

}  // karuta
