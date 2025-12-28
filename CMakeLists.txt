cmake_minimum_required(VERSION 3.13)

# Create the library
add_library(VMA451 INTERFACE)

# Add source files
target_sources(VMA451 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/VMA451.cpp
)

# Add include directories
target_include_directories(VMA451 INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}
)

# Link dependencies
target_link_libraries(VMA451 INTERFACE pico_stdlib hardware_gpio)
