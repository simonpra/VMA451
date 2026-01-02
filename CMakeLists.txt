cmake_minimum_required(VERSION 3.13)

# Create the library
add_library(VMA451 INTERFACE)

# Add source files
# Adding VMA451_charset.c to be available as "#include VMA451_charset.h"
target_sources(VMA451 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src/VMA451.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/VMA451_charset.c
)

# Add include directories
target_include_directories(VMA451 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/src
)

# Link dependencies
target_link_libraries(VMA451 INTERFACE pico_stdlib hardware_gpio)
