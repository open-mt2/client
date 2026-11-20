add_executable(test_player_owner
    test_player_owner.cpp
)

target_link_libraries(test_player_owner
    PRIVATE
        client_ui
        pthread
        dl
)

target_include_directories(test_player_owner
    PRIVATE
        ${CLIENT_INCLUDE_DIR}
        ${PYTHON_INCLUDE_DIRS}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include")
    target_include_directories(test_player_owner PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include)
endif()

if(UNIX)
    target_compile_definitions(test_player_owner PRIVATE OPENMT2_PLATFORM_LINUX=1 OPENMT2_RENDERER_GL=1 OPENMT2_SDL_WINDOW=1 _GNU_SOURCE)
endif()

add_test(NAME PlayerOwnerTests COMMAND test_player_owner)
