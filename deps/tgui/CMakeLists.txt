add_library(TGUI INTERFACE)

target_include_directories(TGUI INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(TGUI INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/lib/libtgui.so")