
pkg_check_variable(wayland-scanner wayland_scanner)

function(use_wayland_scanner _target _xml)
  get_filename_component(_protocol ${_xml} NAME_WE)
  string(TOUPPER ${_target} _target_upper)
  string(REPLACE "-" "_" _target_upper ${_target_upper})

  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/${_protocol}-protocol.c
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS "${_xml}"
    COMMAND "${WAYLAND_SCANNER_WAYLAND_SCANNER}" "code" "${_xml}" "${CMAKE_SOURCE_DIR}/${_protocol}-protocol.c"
    )
  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/${_protocol}-server-protocol.h
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS "${_xml}"
    COMMAND "${WAYLAND_SCANNER_WAYLAND_SCANNER}" "server-header" "${_xml}" "${CMAKE_SOURCE_DIR}/${_protocol}-server-protocol.h"
    )
  add_custom_target("${_target}-protocol"
    SOURCES ${CMAKE_SOURCE_DIR}/${_protocol}-protocol.c ${CMAKE_SOURCE_DIR}/${_protocol}-server-protocol.h
    )

  set("${_target_upper}_PROTOCOL_SOURCES" ${CMAKE_SOURCE_DIR}/${_protocol}-protocol.c PARENT_SCOPE)
endfunction()
