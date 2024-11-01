cmake_minimum_required(VERSION 3.10)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
include_directories(${CMAKE_SOURCE_DIR}/include)
if(MSVC)
    add_compile_options(/W4 /O2)
else() # UNIX or MINGW
    add_compile_options(-Wall -Wextra -s -O2)
endif()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

file(GLOB SOURCES *.c)
foreach(SOURCE ${SOURCES})
    get_filename_component(TARGET_NAME ${SOURCE} NAME_WE)
    add_executable(${TARGET_NAME} ${SOURCE})
    target_link_libraries(${TARGET_NAME} PRIVATE ${CMAKE_BINARY_DIR}/tests/mathy)
endforeach()
