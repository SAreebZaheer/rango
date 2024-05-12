cmake_minimum_required(VERSION 3.27)
project(rango)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

include_directories(.)
include_directories(libraries)

get_filename_component(SOURCE_DIR ABSOLUTE PATH ${PROJECT_SOURCE_DIR}/HTTP)
get_filename_component(BINARY_DIR ABSOLUTE PATH ${PROJECT_BINARY_DIR}/HTTP)


file(GLOB source_files RELATIVE HTTP/ "*")
file(COPY ${source_files} DESTINATION ${BINARY_DIR}/HTTP)


add_executable(rango
        libraries/errorCodes.hpp
        libraries/pets.hpp
        libraries/unitTests.hpp
        libraries/users.hpp
        Server/server.hpp
        rango.cpp
        rango.hpp)

target_link_libraries(rango PRIVATE ws2_32)