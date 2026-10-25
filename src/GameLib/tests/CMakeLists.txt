add_executable(test_gamelib
    test_gamelib.cpp
)

target_link_libraries(test_gamelib
    PRIVATE
        GameLib
        EffectLib
        PRTerrainLib
        SpeedTreeLib
        EterGrnLib
        MilesLib
        EterLib
        EterPack
        EterImageLib
        SphereLib
        EterLocale
        EterBase
        pthread
        dl
)

target_include_directories(test_gamelib
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../include/GameLib
)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include")
    target_include_directories(test_gamelib PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../../extern/include)
endif()

if(UNIX)
    target_compile_definitions(test_gamelib PRIVATE __LINUX__ _GNU_SOURCE)
endif()

add_test(NAME GameLib_SmokeTests COMMAND test_gamelib)
