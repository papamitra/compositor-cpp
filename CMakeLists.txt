
cmake_minimum_required(VERSION 2.8)
project (compositor)

include(cmake/PkgConfigVars.cmake)
include(cmake/CreateKarutaInterface.cmake)
include(cmake/UseWaylandScanner.cmake)

set( SOURCE
  ${CMAKE_SOURCE_DIR}/main.cpp
  ${CMAKE_SOURCE_DIR}/client.cpp
  ${CMAKE_SOURCE_DIR}/compositor.cpp
  ${CMAKE_SOURCE_DIR}/display.cpp
  ${CMAKE_SOURCE_DIR}/resource.cpp
  ${CMAKE_SOURCE_DIR}/shell.cpp
  ${CMAKE_SOURCE_DIR}/surface.cpp
  ${CMAKE_SOURCE_DIR}/buffer.cpp
  ${CMAKE_SOURCE_DIR}/xdg_shell.cpp
)

find_package(PkgConfig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -g -O0")

pkg_check_modules(WAYLAND_SERVER REQUIRED wayland-server)

pkg_check_variable(wayland-scanner pkgdatadir)

create_karuta_interface(wayland "${WAYLAND_SCANNER_PKGDATADIR}/wayland.xml")
use_wayland_scanner(xdg-shell "wayland-protocols/unstable/xdg-shell/xdg-shell-unstable-v6.xml")
create_karuta_interface(xdg-shell "wayland-protocols/unstable/xdg-shell/xdg-shell-unstable-v6.xml")

message("${XDG_SHELL_PROTOCOL_SOURCES}")

add_executable(compositor
  ${XDG_SHELL_PROTOCOL_SOURCES}
  ${WAYLAND_KARUTA_INTERFACE_SOURCES}
  ${XDG_SHELL_KARUTA_INTERFACE_SOURCES}
  ${SOURCE}
)

target_link_libraries(compositor
  ${WAYLAND_SERVER_LIBRARIES}
  -ldl
  )

add_dependencies(compositor
  xdg-shell-protocol
  wayland-karuta-interface
  xdg-shell-karuta-interface
)


add_subdirectory(backend/x11)
