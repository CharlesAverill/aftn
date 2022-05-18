cmake_minimum_required(VERSION 3.14)
project(aftn LANGUAGES C VERSION 0.0.2)

# https://cmake.org/cmake/help/latest/prop_tgt/C_STANDARD.html
string(COMPARE EQUAL "${CMAKE_C_STANDARD}" "" no_cmake_c_standard_set)
if(no_cmake_cxx_standard_set)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)
    message(STATUS "Using default C standard ${CMAKE_C_STANDARD}")
else()
    message(STATUS "Using user specified C standard ${CMAKE_C_STANDARD}")
endif()

option(DebugBuild "DebugBuild" OFF)

if(DebugBuild)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type on single-configuration generators")

    if(Win32)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}\bin")
        add_compile_definitions(GAME_DATA="${CMAKE_SOURCE_DIR}\game_data")
    else()
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
        add_compile_definitions(GAME_DATA="${CMAKE_SOURCE_DIR}/game_data")
    endif(Win32)

    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}" CACHE PATH "Install directory used by install()" FORCE)
else()
    if(Win32)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "\Program Files\aftn")
        set(CMAKE_INSTALL_PREFIX "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" CACHE PATH "Install directory used by install()")
        add_compile_definitions(GAME_DATA="${CMAKE_INSTALL_PREFIX}\game_data")
    else()
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/usr/games")
        set(CMAKE_INSTALL_PREFIX "/usr/share/games/aftn" CACHE PATH "Install directory used by install()" FORCE)
        add_compile_definitions(GAME_DATA="${CMAKE_INSTALL_PREFIX}/game_data")
    endif()

    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type on single-configuration generators")

    message("Call with -DInstallMaterials=ON to install copies of the game manual and game pieces to ${CMAKE_INSTALL_PREFIX}/game_data/materials")
    option(InstallMaterials "InstallMaterials" OFF)

    # Copy maps to install directory
    install(FILES
            "${CMAKE_SOURCE_DIR}/game_data/maps/default"
            "${CMAKE_SOURCE_DIR}/game_data/maps/format.txt"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/game_data")
    install(FILES
            "${CMAKE_SOURCE_DIR}/game_data/banner.txt"
            DESTINATION "${CMAKE_INSTALL_PREFIX}/game_data/")

    if(InstallMaterials)
        install(FILES
                "${CMAKE_SOURCE_DIR}/game_data/materials"
                DESTINATION "${CMAKE_INSTALL_PREFIX}/game_data")
    endif(InstallMaterials)
endif(DebugBuild)

unset(DebugBuild CACHE)
unset(InstallMaterials CACHE)

# Configuration
configure_file (
  "${CMAKE_SOURCE_DIR}/include/config.h.in"
  "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.h"
)
include_directories("${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

# Includes
include_directories("${CMAKE_SOURCE_DIR}/include")

# Sources
file(GLOB_RECURSE SRCS "${CMAKE_SOURCE_DIR}/src/*.c" "${CMAKE_SOURCE_DIR}/src/map/*.c")

# Final
add_executable(aftn ${SRCS})

if(Win32)
    install(TARGETS aftn DESTINATION "${CMAKE_INSTALL_PREFIX}")
else()
    install(TARGETS aftn DESTINATION "/usr/games")
endif(Win32)
