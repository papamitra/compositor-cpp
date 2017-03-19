cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(DISABLE_CXX_WARNINGS   "-Wno-unused-parameter -Wno-unused-function -Wno-missing-field-initializers")
set(CMAKE_C_FLAGS          "-g -O0 -fprofile-arcs -ftest-coverage -Wall -W")
set(CMAKE_CXX_FLAGS        "-g -O0 -fprofile-arcs -ftest-coverage -Wall ${DISABLE_CXX_WARNINGS} -std=c++17")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

find_package(PkgConfig)
pkg_check_modules(WAYLAND_SERVER REQUIRED wayland-server)


include_directories(
  ../
  )

file(GLOB SOURCE
  src/*.cpp
  )

add_executable(unit_test ${SOURCE})
target_link_libraries(unit_test
  ${WAYLAND_SERVER_LIBRARIES}
  -lgtest
  -lgtest_main
  -lpthread
  )
