add_executable(test_map
    test_map.cpp
)

target_link_libraries(test_map
    PRIVATE
        client_map
        pthread
        dl
)

target_include_directories(test_map
    PRIVATE
        ${CLIENT_INCLUDE_DIR}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include")
    target_include_directories(test_map PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include)
endif()

if(UNIX)
    target_compile_definitions(test_map PRIVATE OPENMT2_PLATFORM_LINUX=1 OPENMT2_RENDERER_GL=1 OPENMT2_SDL_WINDOW=1 _GNU_SOURCE)
endif()

target_compile_options(test_map PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-UNDEBUG>)

add_test(NAME Map_QueryTests COMMAND test_map)
