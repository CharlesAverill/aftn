cmake_minimum_required(VERSION 3.14)
project(aftn C)

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

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type on single-configuration generators")

set(CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Install directory used by install()")
message("Call with -DInstallMaterials=ON to install copies of the game manual and game pieces to ${CMAKE_INSTALL_PREFIX}/share/games/aftn/game_data/materials")
option(InstallMaterials "InstallMaterials" OFF)

# Copy maps to install directory
install(FILES
    ${CMAKE_SOURCE_DIR}/game_data/maps/default
    ${CMAKE_SOURCE_DIR}/game_data/maps/format.txt
    DESTINATION share/games/aftn/game_data/maps)
install(FILES ${CMAKE_SOURCE_DIR}/game_data/banner.txt DESTINATION share/games/aftn/game_data)

if(InstallMaterials)
    install(FILES ${CMAKE_SOURCE_DIR}/game_data/materials DESTINATION share/games/aftn/game_data)
    add_compile_definitions(DEFAULT_MAP="${CMAKE_INSTALL_PREFIX}/share/games/aftn/game_data/maps/default"
                            BANNER="${CMAKE_INSTALL_PREFIX}/share/games/aftn/game_data/banner.txt"
    )
else()
    add_compile_definitions(DEFAULT_MAP="${CMAKE_SOURCE_DIR}/game_data/maps/default"
                            BANNER="${CMAKE_SOURCE_DIR}/game_data/banner.txt"
    )
endif(InstallMaterials)
unset(InstallMaterials CACHE)

# Includes
include_directories(${CMAKE_SOURCE_DIR}/include)

# Sources
file(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/src/*.c ${CMAKE_SOURCE_DIR}/src/map/*.c)

# Final
add_executable(aftn ${SRCS})

install(TARGETS aftn DESTINATION bin)
