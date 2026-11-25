# Headless check of the script runtime selection on .NET (no window/GPU).
if(NOT TARGET neoeter_dotnet_host)
    return()
endif()

set(_entry_dir "${CMAKE_CURRENT_BINARY_DIR}/entry")
file(GLOB _entry_sources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/ScriptRuntimeTests/*.cs"
    "${CMAKE_CURRENT_SOURCE_DIR}/ScriptRuntimeTests/*.csproj")
add_custom_command(
    OUTPUT "${_entry_dir}/ScriptRuntimeTests.dll"
    COMMAND "${DOTNET_EXECUTABLE}" build "${CMAKE_CURRENT_SOURCE_DIR}/ScriptRuntimeTests/ScriptRuntimeTests.csproj"
            -c ${NEOETER_DOTNET_CONFIGURATION} -o "${_entry_dir}" "-p:NeoeterDotNetDir=${NEOETER_DOTNET_SOURCE_DIR}"
            --nologo -v quiet
    DEPENDS ${_entry_sources} neoeter_dotnet_managed
    COMMENT "Building ScriptRuntimeTests (C#)"
    VERBATIM)
add_custom_target(client_script_runtime_entry ALL DEPENDS "${_entry_dir}/ScriptRuntimeTests.dll")

add_executable(test_script_runtime test_script_runtime.cpp)
target_link_libraries(test_script_runtime PRIVATE client_application neoeter_dotnet_host neoeter_io pthread dl)
add_dependencies(test_script_runtime client_script_runtime_entry)

# CDotNetRuntime looks for Neoeter.Scripting in <exe dir>/dotnet.
add_custom_command(TARGET test_script_runtime POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${NEOETER_DOTNET_OUTPUT_DIR}" "$<TARGET_FILE_DIR:test_script_runtime>/dotnet")

add_test(NAME test_script_runtime COMMAND test_script_runtime "${_entry_dir}/ScriptRuntimeTests.dll")
if(NEOETER_DOTNET_ROOT)
    set_tests_properties(test_script_runtime PROPERTIES ENVIRONMENT "DOTNET_ROOT=${NEOETER_DOTNET_ROOT}")
endif()
