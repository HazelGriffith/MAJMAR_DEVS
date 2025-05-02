cmake_minimum_required(VERSION 3.16)

project(MAJMAR)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# Regular CMake project setup for non-ESP32


# Add required libraries and include directories
add_library(data_structures data_structures/evacInfo.cpp data_structures/heloInfo.cpp)
target_include_directories(data_structures PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/data_structures  
)

add_executable(main main.cpp)

target_include_directories(main PRIVATE "." "include" $ENV{CADMIUM})

# Non-ESP32 specific compile options
target_compile_options(main PUBLIC -std=gnu++2b)

target_link_libraries(main data_structures)