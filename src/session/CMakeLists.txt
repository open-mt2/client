cmake_minimum_required(VERSION 3.16)
project(client_session LANGUAGES C CXX)

file(GLOB_RECURSE SESSION_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(client_session OBJECT ${SESSION_SOURCES})

target_include_directories(client_session
    PUBLIC
        $<BUILD_INTERFACE:${CLIENT_INCLUDE_DIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

find_library(PYTHON_LIBRARIES NAMES python2.7 python27 libpython2.7 libpython27
    HINTS
        ${CMAKE_CURRENT_SOURCE_DIR}/../../extern/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/../extern/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/extern/lib
)
find_path(PYTHON_INCLUDE_DIRS NAMES Python.h
    HINTS
        ${PYTHON_INCLUDE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include/python2.7
        ${CMAKE_CURRENT_SOURCE_DIR}/../extern/include/python2.7
        /usr/include/python2.7
)
target_include_directories(client_session SYSTEM PRIVATE ${PYTHON_INCLUDE_DIRS})

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include")
    target_include_directories(client_session PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include>)
endif()

if(UNIX)
    target_compile_definitions(client_session PUBLIC
        OPENMT2_PLATFORM_LINUX=1
        OPENMT2_RENDERER_GL=1
        OPENMT2_SDL_WINDOW=1
        _GNU_SOURCE
    )
endif()

if(NOT DEFINED OPENMT2_CLIENT_VERSION)
    set(OPENMT2_CLIENT_VERSION "unknown")
endif()
target_compile_definitions(client_session PRIVATE OPENMT2_CLIENT_VERSION="${OPENMT2_CLIENT_VERSION}")

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Distribute" OR NOT CMAKE_BUILD_TYPE)
    target_compile_definitions(client_session PUBLIC _DISTRIBUTE)
endif()

target_link_libraries(client_session
    PUBLIC
        client_protocol
        client_game
        neoeter_python_bindings
        neoeter_python_host
        neoeter_effect
        neoeter_terrain
        neoeter_model_speedtree
        neoeter_model_grn
        neoeter_audio
        neoeter_render
        neoeter_platform_math
        neoeter_image
        neoeter_io
        neoeter_platform_locale
        neoeter_platform_base
        ${PYTHON_LIBRARIES}
        $<$<PLATFORM_ID:Linux>:pthread>
        $<$<PLATFORM_ID:Linux>:dl>
)
