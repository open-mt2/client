cmake_minimum_required(VERSION 3.16)
project(client_script LANGUAGES CXX)

file(GLOB_RECURSE SCRIPT_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(FILTER SCRIPT_SOURCES EXCLUDE REGEX "/tests/")

add_library(client_script STATIC ${SCRIPT_SOURCES})
add_library(Client::Script ALIAS client_script)

target_include_directories(client_script
    PUBLIC
        $<BUILD_INTERFACE:${CLIENT_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(client_script
    PUBLIC
        client_ui
)

if(BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    add_subdirectory(tests)
endif()
