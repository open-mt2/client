cmake_minimum_required(VERSION 3.16)
project(client_application LANGUAGES CXX)

file(GLOB_RECURSE APPLICATION_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)
list(FILTER APPLICATION_SOURCES EXCLUDE REGEX "/tests/")

add_library(client_application STATIC ${APPLICATION_SOURCES})
add_library(Client::Application ALIAS client_application)

target_include_directories(client_application
    PUBLIC
        $<BUILD_INTERFACE:${CLIENT_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(client_application
    PUBLIC
        client_ui
)

if(DEFINED OPENMT2_CLIENT_VERSION)
    target_compile_definitions(client_application PRIVATE OPENMT2_CLIENT_VERSION="${OPENMT2_CLIENT_VERSION}")
endif()
