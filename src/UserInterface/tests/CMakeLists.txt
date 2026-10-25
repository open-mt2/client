add_executable(test_userinterface
    test_userinterface.cpp
)

target_link_libraries(test_userinterface
    PRIVATE
        UserInterface
        pthread
        dl
)

target_include_directories(test_userinterface
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/UserInterface
        ${PYTHON_INCLUDE_DIRS}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include")
    target_include_directories(test_userinterface PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include)
endif()

if(UNIX)
    target_compile_definitions(test_userinterface PRIVATE __LINUX__ _GNU_SOURCE)
endif()

add_test(NAME UserInterface_SmokeTests COMMAND test_userinterface)
