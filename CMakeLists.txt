cmake_minimum_required(VERSION 3.20)
project(Client VERSION 0.1.5 LANGUAGES C CXX)

set(OPENMT2_CLIENT_VERSION "${PROJECT_VERSION}")

# Game layer: GameLib + UserInterface + CWebBrowser. One header root, one
# directory per package. Depends on neoeter and neoeter-python.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CLIENT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories("${CLIENT_INCLUDE_DIR}")

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(CLIENT_STANDALONE_DEFAULT ON)
else()
    set(CLIENT_STANDALONE_DEFAULT OFF)
endif()

option(CLIENT_STANDALONE "Build the client on its own" ${CLIENT_STANDALONE_DEFAULT})
option(CLIENT_DOWNLOAD_NEOETER "Download a prebuilt Neoeter release instead of building it" ${CLIENT_STANDALONE_DEFAULT})

set(NEOETER_VERSION "latest" CACHE STRING "Neoeter release version, or 'latest'")
set(NEOETER_SHA256 "" CACHE STRING "SHA256 of the Neoeter release archive")
set(NEOETER_ASSET_PLATFORM "" CACHE STRING "Override the Neoeter release platform suffix")
set(NEOETER_GIT_TAG "main" CACHE STRING "neoeter git tag when building from source")
set(NEOETER_PYTHON_GIT_TAG "main" CACHE STRING "neoeter-python git tag")

if(CLIENT_STANDALONE)
    include(FetchContent)

    if(CLIENT_DOWNLOAD_NEOETER)
        if(NEOETER_VERSION STREQUAL "latest")
            set(_neoeter_latest_json "${CMAKE_BINARY_DIR}/_deps/neoeter-latest.json")
            file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/_deps")
            file(DOWNLOAD "https://api.github.com/repos/open-mt2/neoeter/releases/latest"
                "${_neoeter_latest_json}"
                HTTPHEADER "Accept: application/vnd.github+json"
                TLS_VERIFY ON
                STATUS _neoeter_latest_status)
            list(GET _neoeter_latest_status 0 _neoeter_latest_code)
            if(NOT _neoeter_latest_code EQUAL 0)
                list(GET _neoeter_latest_status 1 _neoeter_latest_message)
                message(FATAL_ERROR "Could not resolve the latest Neoeter release: ${_neoeter_latest_message}")
            endif()
            file(READ "${_neoeter_latest_json}" _neoeter_latest_text)
            string(JSON _neoeter_latest_tag ERROR_VARIABLE _neoeter_latest_error
                GET "${_neoeter_latest_text}" tag_name)
            if(_neoeter_latest_error)
                message(FATAL_ERROR "Could not parse the latest Neoeter release: ${_neoeter_latest_error}")
            endif()
            string(REGEX REPLACE "^v" "" NEOETER_VERSION "${_neoeter_latest_tag}")
            message(STATUS "Neoeter latest release: ${NEOETER_VERSION}")
        endif()

        if(NOT NEOETER_ASSET_PLATFORM)
            if(WIN32)
                set(NEOETER_ASSET_PLATFORM "windows-x86_64")
            elseif(APPLE)
                set(NEOETER_ASSET_PLATFORM "darwin-x86_64")
            else()
                set(NEOETER_ASSET_PLATFORM "linux-x86_64")
            endif()
        endif()

        set(_neoeter_asset "neoeter-${NEOETER_VERSION}-${NEOETER_ASSET_PLATFORM}.tar.gz")
        set(_neoeter_url "https://github.com/open-mt2/neoeter/releases/download/v${NEOETER_VERSION}/${_neoeter_asset}")
        set(_neoeter_root "${CMAKE_BINARY_DIR}/_deps")
        set(_neoeter_archive "${_neoeter_root}/${_neoeter_asset}")
        set(_neoeter_dir "${_neoeter_root}/neoeter-${NEOETER_VERSION}-${NEOETER_ASSET_PLATFORM}")

        if(NOT EXISTS "${_neoeter_dir}/lib/cmake/Neoeter/NeoeterConfig.cmake")
            file(MAKE_DIRECTORY "${_neoeter_root}")

            if(NOT NEOETER_SHA256)
                file(DOWNLOAD "${_neoeter_url}.sha256" "${_neoeter_archive}.sha256"
                    TLS_VERIFY ON
                    STATUS _neoeter_sha_status)
                list(GET _neoeter_sha_status 0 _neoeter_sha_code)
                if(_neoeter_sha_code EQUAL 0)
                    file(READ "${_neoeter_archive}.sha256" _neoeter_sha_text)
                    string(REGEX MATCH "^[0-9a-fA-F]+" NEOETER_SHA256 "${_neoeter_sha_text}")
                endif()
            endif()

            if(NEOETER_SHA256)
                file(DOWNLOAD "${_neoeter_url}" "${_neoeter_archive}"
                    EXPECTED_HASH SHA256=${NEOETER_SHA256}
                    TLS_VERIFY ON
                    STATUS _neoeter_status)
            else()
                file(DOWNLOAD "${_neoeter_url}" "${_neoeter_archive}"
                    TLS_VERIFY ON
                    STATUS _neoeter_status)
            endif()

            list(GET _neoeter_status 0 _neoeter_status_code)
            if(NOT _neoeter_status_code EQUAL 0)
                list(GET _neoeter_status 1 _neoeter_status_message)
                message(FATAL_ERROR "Neoeter download failed: ${_neoeter_status_message} (${_neoeter_url})")
            endif()

            file(ARCHIVE_EXTRACT INPUT "${_neoeter_archive}" DESTINATION "${_neoeter_root}")
        endif()

        list(PREPEND CMAKE_PREFIX_PATH "${_neoeter_dir}")
        find_package(Neoeter CONFIG REQUIRED)
        if(DEFINED Neoeter_VERSION AND NOT Neoeter_VERSION STREQUAL NEOETER_VERSION)
            message(WARNING "Neoeter package version ${Neoeter_VERSION} does not match the requested release ${NEOETER_VERSION}")
        endif()
    else()
        FetchContent_Declare(neoeter
            GIT_REPOSITORY https://github.com/open-mt2/neoeter.git
            GIT_TAG ${NEOETER_GIT_TAG})
    endif()

    FetchContent_Declare(neoeter-python
        GIT_REPOSITORY https://github.com/open-mt2/neoeter-python.git
        GIT_TAG ${NEOETER_PYTHON_GIT_TAG})

    set(BUILD_TESTING_SAVED ${BUILD_TESTING})
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    if(CLIENT_DOWNLOAD_NEOETER)
        FetchContent_MakeAvailable(neoeter-python)
    else()
        FetchContent_MakeAvailable(neoeter neoeter-python)
    endif()
    set(BUILD_TESTING ${BUILD_TESTING_SAVED} CACHE BOOL "" FORCE)
endif()

# The browser stub has no dependencies; the UI links both.
foreach(_package IN ITEMS webbrowser game ui)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${_package}/CMakeLists.txt")
        add_subdirectory("src/${_package}" "${CMAKE_BINARY_DIR}/src/${_package}")
    else()
        message(STATUS "Client: src/${_package} not imported yet")
    endif()
endforeach()
