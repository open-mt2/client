cmake_minimum_required(VERSION 3.16)
project(client_protocol LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED CLIENT_INCLUDE_DIR)
    get_filename_component(CLIENT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../include" ABSOLUTE)
endif()

file(GLOB_RECURSE PROTOCOL_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(FILTER PROTOCOL_SOURCES EXCLUDE REGEX "/tests/")

add_library(client_protocol STATIC ${PROTOCOL_SOURCES})

target_include_directories(client_protocol
    PUBLIC
        $<BUILD_INTERFACE:${CLIENT_INCLUDE_DIR}>
)

if(BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()
