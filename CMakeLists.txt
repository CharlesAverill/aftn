cmake_minimum_required(VERSION 3.14)
project(aftn C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_BUILD_TYPE Debug)

message("Call with -DInstallMaterials=ON to install copies of the game manual and game pieces to /var/games/aftn/game_data/materials")
option(InstallMaterials "InstallMaterials" OFF)

# Copy maps to /var/games/aftn
file(MAKE_DIRECTORY /var/games/aftn)
file(MAKE_DIRECTORY /var/games/aftn/game_data)
file(COPY ${CMAKE_SOURCE_DIR}/game_data/maps DESTINATION /var/games/aftn/game_data)
file(COPY ${CMAKE_SOURCE_DIR}/game_data/banner.txt DESTINATION /var/games/aftn/game_data)

if(InstallMaterials)
    file(COPY ${CMAKE_SOURCE_DIR}/game_data/materials DESTINATION /var/games/aftn/game_data)
endif(InstallMaterials)
unset(InstallMaterials CACHE)

# Includes
include_directories(${CMAKE_SOURCE_DIR}/include)

# Sources
file(GLOB_RECURSE SRCS ${CMAKE_SOURCE_DIR}/src/*.c ${CMAKE_SOURCE_DIR}/src/map/*.c)

# Final
add_executable(aftn ${SRCS})
