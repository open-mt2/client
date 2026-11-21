add_executable(test_script_seam
    test_script_seam.cpp
)

add_executable(test_script_python_host
    test_script_python_host.cpp
)

target_link_libraries(test_script_seam
    PRIVATE
        client_script
)

target_link_libraries(test_script_python_host
    PRIVATE
        client_script
)

target_include_directories(test_script_seam
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CLIENT_INCLUDE_DIR}
)

target_include_directories(test_script_python_host
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CLIENT_INCLUDE_DIR}
)

target_compile_options(test_script_seam PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-UNDEBUG>)
target_compile_options(test_script_python_host PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-UNDEBUG>)

add_test(NAME ScriptSeamTests COMMAND test_script_seam)
add_test(NAME ScriptPythonHostTests COMMAND test_script_python_host)
