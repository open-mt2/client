cmake_minimum_required(VERSION 3.16)
project(client_utils LANGUAGES CXX)

file(GLOB_RECURSE UTILS_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(FILTER UTILS_SOURCES EXCLUDE REGEX "/tests/")

add_library(client_utils STATIC ${UTILS_SOURCES})
add_library(Client::Utils ALIAS client_utils)

target_include_directories(client_utils
    PUBLIC
        $<BUILD_INTERFACE:${CLIENT_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(client_utils
    PUBLIC
        neoeter_platform_base
)

if(UNIX)
    target_compile_definitions(client_utils PUBLIC
        OPENMT2_PLATFORM_LINUX=1
        OPENMT2_RENDERER_GL=1
        OPENMT2_SDL_WINDOW=1
        _GNU_SOURCE
    )
endif()
