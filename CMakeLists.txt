cmake_minimum_required(VERSION 3.27)
project(rango)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")

include_directories(.)
include_directories(libraries)

add_executable(rango
        libraries/errorCodes.hpp
        libraries/pets.hpp
        libraries/unitTests.hpp
        libraries/users.hpp
        HTTP/server.hpp
        rango.cpp
        rango.hpp)

target_link_libraries(rango PRIVATE ws2_32)