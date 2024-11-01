cmake_minimum_required(VERSION 3.10)

project(Mathy VERSION 1.0.0)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR}/include)
add_definitions(-DMATHY_EXPORTS)
if(MSVC)
    add_compile_options(/W4)
else() # UNIX or MINGW
    add_compile_options(-Wall -Wextra)
endif()

set(SOURCE ${CMAKE_SOURCE_DIR}/src/mathy.c)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        add_compile_options(/Zi /Od)
    else() # UNIX or MINGW
        add_compile_options(-g -O0)
    endif()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/debug/${PROJECT_VERSION})
else()
    if(MSVC)
        add_compile_options(/O2)
    else() # UNIX or MINGW
        add_compile_options(-s -O2)
    endif()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/release/${PROJECT_VERSION})
endif()

add_library(mathy SHARED $(SOURCE))
