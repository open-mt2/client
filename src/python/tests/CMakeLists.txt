add_executable(test_python_client_api
    test_python_client_api.cpp
)

target_link_libraries(test_python_client_api
    PRIVATE
        client_script
        client_ui
        pthread
        dl
)

target_include_directories(test_python_client_api
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CLIENT_INCLUDE_DIR}
        ${PYTHON_INCLUDE_DIRS}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include")
    target_include_directories(test_python_client_api PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include)
endif()

if(UNIX)
    target_compile_definitions(test_python_client_api PRIVATE OPENMT2_PLATFORM_LINUX=1 OPENMT2_RENDERER_GL=1 OPENMT2_SDL_WINDOW=1 _GNU_SOURCE)
endif()

target_compile_options(test_python_client_api PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-UNDEBUG>)

add_test(NAME test_python_client_api
    COMMAND test_python_client_api
        ${CMAKE_CURRENT_SOURCE_DIR}/data/python_api_client.txt
)

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
