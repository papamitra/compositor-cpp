
cmake_minimum_required(VERSION 2.8)
project (compositor)

file(GLOB SOURCE
  ${CMAKE_SOURCE_DIR}/*.cpp
)

add_executable(compositor ${SOURCE})
add_dependencies(compositor server-protocol)

find_package(PkgConfig)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++14 -g -O0")

pkg_check_modules(WAYLAND_SERVER REQUIRED wayland-server)

target_link_libraries(compositor
  ${WAYLAND_SERVER_LIBRARIES}
  -ldl
  )

# FIXME: wayland.xml path
add_custom_command(
  OUTPUT ${CMAKE_SOURCE_DIR}/server-protocol.h ${CMAKE_SOURCE_DIR}/server-protocol.cpp
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  DEPENDS ${CMAKE_SOURCE_DIR}/create_interface.py
  COMMAND "python3" "create_interface.py" "/usr/share/wayland/wayland.xml" "server-protocol"
)

add_custom_target(server-protocol
    SOURCES ${CMAKE_SOURCE_DIR}/server-protocol.h ${CMAKE_SOURCE_DIR}/server-protocol.cpp
)

add_subdirectory(backend/x11)
