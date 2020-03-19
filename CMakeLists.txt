# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

cmake_minimum_required(VERSION 3.5)
project(BuildoLand)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_DIR "F:/SFML/SFML-2.5.1-sources/SFML-2.5.1-build")

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

option(BUILD_SERVER "Build server-side" ON)

include_directories(
    client-source
    client-source/Gui
    client-source/Res
    client-source/Settings
    client-source/States
    client-source/Utils
    client-source/World
    common-source
    common-source/Blocks
    common-source/Blocks/GameBlocks
    common-source/Entities
    common-source/Entities/GameEntities
    common-source/Entities/GameTileEntities
    common-source/Grounds
    common-source/Grounds/GameGrounds
    common-source/Items
    common-source/Items/GameItems
    common-source/Networking
    common-source/Utils
    server-source/Server
    server-source/Utils
    server-source/World
)

file(GLOB_RECURSE CLIENT_SRCS client-source/*.cpp client-source/*.cc client-source/*.c client-source/*.h)
file(GLOB_RECURSE COMMON_SRCS common-source/*.cpp common-source/*.cc common-source/*.c common-source/*.h)
file(GLOB_RECURSE SERVER_SRCS server-source/*.cpp server-source/*.cc server-source/*.c server-source/*.h)

if (NOT BUILD_SERVER)
    set (SRCS
        ${CLIENT_SRCS}
        ${COMMON_SRCS}
    )
    add_definitions(-DCLIENT_SIDE)
else()
    set (SRCS
        ${SERVER_SRCS}
        ${COMMON_SRCS}
    )
endif()


add_executable(${CMAKE_PROJECT_NAME} ${SRCS})

target_link_libraries(${CMAKE_PROJECT_NAME} sfml-graphics sfml-audio sfml-network)
