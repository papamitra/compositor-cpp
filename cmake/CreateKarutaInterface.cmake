# - create source code from wayland protocol xml file
#
# usage:
#   create_karuta_intarface(<TARGET> <PROTOCOL NAME> <PROTOCOL FILE>)
#
 
function(create_karuta_interface _target _xml)
  get_filename_component(_protocol ${_xml} NAME_WE)
  string(TOUPPER ${_target} _target_upper)
  string(REPLACE "-" "_" _target_upper ${_target_upper})

  add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/${_protocol}_karuta_server.h ${CMAKE_SOURCE_DIR}/${_protocol}_karuta_server.cpp
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    DEPENDS ${CMAKE_SOURCE_DIR}/create_interface.py
    COMMAND "python3" "create_interface.py" "${_xml}"
    )
  add_custom_target("${_target}-karuta-interface"
    SOURCES ${CMAKE_SOURCE_DIR}/${_protocol}_karuta_server.h ${CMAKE_SOURCE_DIR}/${_protocol}_karuta_server.cpp
    )

  set("${_target_upper}_KARUTA_INTERFACE_SOURCES" ${CMAKE_SOURCE_DIR}/${_protocol}_karuta_server.cpp PARENT_SCOPE)

endfunction()

