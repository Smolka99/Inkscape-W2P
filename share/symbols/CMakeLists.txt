# SPDX-License-Identifier: GPL-2.0-or-later
file(GLOB _FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.svg" "sjjb/*.svg")

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/symbols.h
  COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/i18n.py ${_FILES} > ${CMAKE_CURRENT_BINARY_DIR}/symbols.h
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/i18n.py ${_FILES}
)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/symbols.h PROPERTIES GENERATED TRUE)
add_custom_target(symbols_h ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/symbols.h)

install(FILES ${_FILES} DESTINATION ${INKSCAPE_SHARE_INSTALL}/symbols)
