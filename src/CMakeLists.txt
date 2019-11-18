# Collect the source files to be compiled
file(GLOB_RECURSE SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/ *.cpp *.hpp)

# Collect the include directories
collect_include_directories(${CMAKE_CURRENT_SOURCE_DIR} PUBLIC_INCLUDES)

# Add the library build target
add_executable(tower-defence ${SRC})

# Include TGUI files
include_directories("${CMAKE_SOURCE_DIR}/deps/TGUI-0.7.8/include")

# Link the library
target_link_libraries(tower-defence
	PUBLIC
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        tgui)