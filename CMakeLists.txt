
cmake_minimum_required(VERSION 2.8)
project (compositor)

file(GLOB SOURCE
  ${CMAKE_SOURCE_DIR}/*.cpp
  ${CMAKE_SOURCE_DIR}/server-protocol.cpp
)

add_executable(compositor ${SOURCE})
#add_dependencies(compositor server-protocol)

find_package(PkgConfig)
find_package(X11 REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++1z -g -O0")

pkg_check_modules(WAYLAND_SERVER REQUIRED wayland-server)
pkg_check_modules(WAYLAND_CLIENT REQUIRED wayland-client)
pkg_check_modules(WAYLAND_EGL REQUIRED wayland-egl)
pkg_check_modules(EGL REQUIRED egl)
pkg_check_modules(GLES2 REQUIRED glesv2)

target_link_libraries(compositor
  ${WAYLAND_SERVER_LIBRARIES}
  ${X11_LIBRARIES}
  ${EGL_LIBRARIES}
  ${GLES2_LIBRARIES}
  )

# FIXME: wayland.xml path
add_custom_command(
  OUTPUT ${CMAKE_SOURCE_DIR}/server-protocol.h ${CMAKE_SOURCE_DIR}/server-protocol.cpp
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND "python3" "create_interface.py" "/usr/share/wayland/wayland.xml" "server-protocol"
)
