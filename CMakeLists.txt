cmake_minimum_required(VERSION 3.20)
project(Client VERSION 0.1.0 LANGUAGES C CXX)

# Game layer: GameLib + UserInterface + the metin2client executable.
# See docs/architecture.md. Depends on neoeter and neoeter-python.

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(CLIENT_STANDALONE "Build the client on its own" OFF)

if(CLIENT_STANDALONE)
    include(FetchContent)
    foreach(_repo IN ITEMS neoeter neoeter-python)
        FetchContent_Declare(${_repo}
            GIT_REPOSITORY https://github.com/open-mt2/${_repo}.git
            GIT_TAG main)
    endforeach()
    set(BUILD_TESTING_SAVED ${BUILD_TESTING})
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(neoeter neoeter-python)
    set(BUILD_TESTING ${BUILD_TESTING_SAVED} CACHE BOOL "" FORCE)
endif()

# CWebBrowser first: UserInterface links it and it has no dependencies.
foreach(_package IN ITEMS CWebBrowser GameLib UserInterface)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/src/${_package}/CMakeLists.txt")
        # Keep build/src/<Package> stable: build.sh copies
        # build/src/UserInterface/metin2client into dist/.
        add_subdirectory("src/${_package}" "${CMAKE_BINARY_DIR}/src/${_package}")
    else()
        message(STATUS "Client: src/${_package} not imported yet")
    endif()
endforeach()

if(TARGET GameLib AND NOT TARGET Client::Game)
    add_library(Client_Game INTERFACE)
    target_link_libraries(Client_Game INTERFACE GameLib)
    add_library(Client::Game ALIAS Client_Game)
endif()

if(TARGET UserInterface AND NOT TARGET Client::UI)
    add_library(Client_UI INTERFACE)
    target_link_libraries(Client_UI INTERFACE UserInterface)
    add_library(Client::UI ALIAS Client_UI)
endif()
