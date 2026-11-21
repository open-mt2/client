cmake_minimum_required(VERSION 3.16)
project(client_map LANGUAGES C CXX)

file(GLOB_RECURSE MAP_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(FILTER MAP_SOURCES EXCLUDE REGEX "/tests/")

add_library(client_map STATIC ${MAP_SOURCES})
add_library(Client::Map ALIAS client_map)

target_include_directories(client_map
    PUBLIC
        $<BUILD_INTERFACE:${CLIENT_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include")
    target_include_directories(client_map PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include>)
endif()

target_link_libraries(client_map
    PUBLIC
        client_gamedata
        neoeter_platform_base
        neoeter_io
        neoeter_render
        neoeter_audio
        neoeter_model_grn
        neoeter_model_speedtree
        neoeter_terrain
        neoeter_effect
)

if(UNIX)
    target_compile_definitions(client_map PUBLIC
        OPENMT2_PLATFORM_LINUX=1
        OPENMT2_RENDERER_GL=1
        OPENMT2_SDL_WINDOW=1
        _GNU_SOURCE
    )
endif()
