cmake_minimum_required(VERSION 3.16)
project(client_gamedata LANGUAGES C CXX)

file(GLOB_RECURSE GAMEDATA_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(FILTER GAMEDATA_SOURCES EXCLUDE REGEX "/tests/")

add_library(client_gamedata STATIC ${GAMEDATA_SOURCES})
add_library(Client::GameData ALIAS client_gamedata)

target_include_directories(client_gamedata
    PUBLIC
        $<BUILD_INTERFACE:${CLIENT_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include")
    target_include_directories(client_gamedata PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include>)
endif()

target_link_libraries(client_gamedata
    PUBLIC
        neoeter_platform_base
        neoeter_io
        neoeter_render
        neoeter_image
        neoeter_model_grn
)

if(UNIX)
    target_compile_definitions(client_gamedata PUBLIC
        OPENMT2_PLATFORM_LINUX=1
        OPENMT2_RENDERER_GL=1
        OPENMT2_SDL_WINDOW=1
        _GNU_SOURCE
    )
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
