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

# TA.1: shared test helpers and always-on asserts (tests/support/TestAssert.h)
foreach(_test_target test_userinterface)
    target_include_directories(${_test_target} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    target_compile_options(${_test_target} PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-UNDEBUG>)
endforeach()
