add_executable(test_gamedata
    test_gamedata.cpp
)

target_link_libraries(test_gamedata
    PRIVATE
        client_gamedata
        neoeter_render
        neoeter_io
        neoeter_image
        neoeter_platform_math
        neoeter_platform_locale
        neoeter_platform_base
        pthread
        dl
)

target_include_directories(test_gamedata
    PRIVATE
        ${CLIENT_INCLUDE_DIR}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include")
    target_include_directories(test_gamedata PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include)
endif()

if(UNIX)
    target_compile_definitions(test_gamedata PRIVATE OPENMT2_PLATFORM_LINUX=1 OPENMT2_RENDERER_GL=1 OPENMT2_SDL_WINDOW=1 _GNU_SOURCE)
endif()

target_compile_options(test_gamedata PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-UNDEBUG>)

add_test(NAME GameData_PortTests COMMAND test_gamedata)
