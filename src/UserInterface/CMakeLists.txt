cmake_minimum_required(VERSION 3.16)
project(UserInterface LANGUAGES C CXX)

if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(USERINTERFACE_STANDALONE ON)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
    option(BUILD_TESTING "Build UserInterface tests" ON)
else()
    set(USERINTERFACE_STANDALONE OFF)
endif()

if(USERINTERFACE_STANDALONE)
    include(FetchContent)

    foreach(dep EterBase:pkg_eterbase EterLocale:pkg_eterlocale EterPack:pkg_eterpack EterImageLib:pkg_eterimagelib SphereLib:pkg_spherelib CWebBrowser:pkg_cwebbrowser EterLib:pkg_eterlib MilesLib:pkg_mileslib EterGrnLib:pkg_etergrnlib SpeedTreeLib:pkg_speedtreelib PRTerrainLib:pkg_prterrainlib EffectLib:pkg_effectlib ScriptLib:pkg_scriptlib EterPythonLib:pkg_eterpythonlib GameLib:pkg_gamelib)
        string(REPLACE ":" ";" dep_list ${dep})
        list(GET dep_list 0 dep_name)
        list(GET dep_list 1 dep_repo)
        if(NOT TARGET ${dep_name})
            if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../${dep_name}/CMakeLists.txt")
                set(BUILD_TESTING_SAVED ${BUILD_TESTING})
                set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
                add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../${dep_name}" "${CMAKE_BINARY_DIR}/deps/${dep_name}")
                set(BUILD_TESTING ${BUILD_TESTING_SAVED} CACHE BOOL "" FORCE)
            else()
                FetchContent_Declare(
                    ${dep_name}
                    GIT_REPOSITORY https://github.com/open-mt2/${dep_repo}.git
                    GIT_TAG main
                )
                set(BUILD_TESTING_SAVED ${BUILD_TESTING})
                set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
                FetchContent_MakeAvailable(${dep_name})
                set(BUILD_TESTING ${BUILD_TESTING_SAVED} CACHE BOOL "" FORCE)
            endif()
        endif()
    endforeach()
endif()

# Locate Python 2.7
find_package(PythonLibs 2.7 QUIET)
if(NOT PYTHON_LIBRARIES)
    find_library(PYTHON_LIBRARIES NAMES python2.7 python27 libpython2.7 libpython27
        HINTS
            ${CMAKE_CURRENT_SOURCE_DIR}/../../extern/lib
            ${CMAKE_CURRENT_SOURCE_DIR}/../extern/lib
            ${CMAKE_CURRENT_SOURCE_DIR}/extern/lib
    )
    find_path(PYTHON_INCLUDE_DIRS NAMES Python.h
        HINTS
            ${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include/python2.7
            ${CMAKE_CURRENT_SOURCE_DIR}/../extern/include/python2.7
            ${CMAKE_CURRENT_SOURCE_DIR}/extern/include/python2.7
            /usr/include/python2.7
    )
endif()

file(GLOB_RECURSE USERINTERFACE_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
list(REMOVE_ITEM USERINTERFACE_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/UserInterface.cpp")

add_library(UserInterface STATIC ${USERINTERFACE_SOURCES})
add_library(OpenMT2::UserInterface ALIAS UserInterface)

target_include_directories(UserInterface
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/UserInterface>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${PYTHON_INCLUDE_DIRS}
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include")
    target_include_directories(UserInterface PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../extern/include>)
endif()

target_link_libraries(UserInterface
    PUBLIC
        GameLib
        EterPythonLib
        ScriptLib
        neoeter_effect
        neoeter_terrain
        neoeter_model_speedtree
        neoeter_model_grn
        neoeter_audio
        neoeter_render
        CWebBrowser
        neoeter_platform_math
        neoeter_image
        neoeter_io
        neoeter_platform_locale
        neoeter_platform_base
        ${PYTHON_LIBRARIES}
        $<$<PLATFORM_ID:Linux>:pthread>
        $<$<PLATFORM_ID:Linux>:dl>
    $<$<PLATFORM_ID:Windows>:dmoguids>
    $<$<PLATFORM_ID:Windows>:strmiids>
)

if(UNIX)
    target_compile_definitions(UserInterface PUBLIC
        OPENMT2_PLATFORM_LINUX=1
        OPENMT2_RENDERER_GL=1
        OPENMT2_SDL_WINDOW=1
        _GNU_SOURCE
    )
endif()

# Master Executable target: metin2client
add_executable(metin2client "${CMAKE_CURRENT_SOURCE_DIR}/src/UserInterface.cpp")
target_link_libraries(metin2client PRIVATE UserInterface)
target_include_directories(metin2client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/UserInterface
    ${PYTHON_INCLUDE_DIRS}
)
if(UNIX)
    target_compile_definitions(metin2client PRIVATE OPENMT2_PLATFORM_LINUX=1 OPENMT2_RENDERER_GL=1 OPENMT2_SDL_WINDOW=1 _GNU_SOURCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "Distribute" OR NOT CMAKE_BUILD_TYPE)
    target_compile_definitions(UserInterface PUBLIC _DISTRIBUTE)
    target_compile_definitions(metin2client PRIVATE _DISTRIBUTE)
endif()

if((USERINTERFACE_STANDALONE AND BUILD_TESTING) OR (NOT USERINTERFACE_STANDALONE AND BUILD_TESTS))
    enable_testing()
    add_subdirectory(tests)
endif()
