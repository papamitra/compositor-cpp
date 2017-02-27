
find_package(PkgConfig)

pkg_check_modules(XCB REQUIRED xcb)
pkg_check_modules(X11 REQUIRED x11)
pkg_check_modules(WAYLAND_EGL REQUIRED wayland-egl)
pkg_check_modules(EGL REQUIRED egl)
pkg_check_modules(GLES2 REQUIRED glesv2)

file(GLOB BACKEND_X11_SOURCE
  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(backend-x11 MODULE ${BACKEND_X11_SOURCE})
set_target_properties(backend-x11 PROPERTIES PREFIX "")

target_include_directories(backend-x11
  PUBLIC ../..
)

target_link_libraries(backend-x11
  ${X11_LIBRARIES}
  ${XCB_LIBRARIES}
  ${EGL_LIBRARIES}
  ${GLES2_LIBRARIES}
)
