cmake_minimum_required(VERSION 3.27)
project(rango)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

include_directories(.)
include_directories(libraries)

get_filename_component(SOURCE_DIR ABSOLUTE PATH ${PROJECT_SOURCE_DIR})
get_filename_component(BINARY_DIR ABSOLUTE PATH ${PROJECT_BINARY_DIR})

# Define source and binary HTTP directories
set(SRC_HTTP_DIR ${SOURCE_DIR}/HTTP)
set(BIN_HTTP_DIR ${BINARY_DIR}/HTTP)

# Copy all files and subdirectories recursively from the HTTP directory to the binary directory
file(COPY ${SRC_HTTP_DIR} DESTINATION ${BIN_HTTP_DIR} FILES_MATCHING PATTERN "*.*")

add_executable(rango
        libraries/errorCodes.hpp
        libraries/pets.hpp
        libraries/unitTests.hpp
        libraries/users.hpp
        Server/server.hpp
        rango.cpp
        rango.hpp)

target_link_libraries(rango PRIVATE ws2_32)
