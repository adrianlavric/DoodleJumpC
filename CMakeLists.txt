cmake_minimum_required(VERSION 3.24)
project(2022_win64)

set(CMAKE_CXX_STANDARD 14)

add_definitions(-D_WINDOWS)

set(SOURCES
        game.cpp
        Framework.h
        )

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/data)

set(LIBS
        ${CMAKE_SOURCE_DIR}/FrameworkRelease_x64.lib
        )

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${LIBS})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/FrameworkRelease_x64.dll
        ${CMAKE_SOURCE_DIR}/libjpeg-9.dll
        ${CMAKE_SOURCE_DIR}/libpng16-16.dll
        ${CMAKE_SOURCE_DIR}/SDL2.dll
        ${CMAKE_SOURCE_DIR}/SDL2_image.dll
        ${CMAKE_SOURCE_DIR}/zlib1.dll
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        )
