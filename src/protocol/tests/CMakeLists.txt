add_executable(test_protocol
    test_protocol.cpp
)

target_link_libraries(test_protocol
    PRIVATE
        client_protocol
)

target_include_directories(test_protocol
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CLIENT_INCLUDE_DIR}
)

target_compile_options(test_protocol PRIVATE $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-UNDEBUG>)

add_test(NAME protocol_wire COMMAND test_protocol)
