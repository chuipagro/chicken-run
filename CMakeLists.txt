cmake_minimum_required(VERSION 3.10)
project(PushYourLuck)

option(BUILD_CLIENT "Build the client" ON)
option(BUILD_SERVER "Build the server" ON)

if (BUILD_SERVER)
    add_subdirectory(server)
endif()

if (BUILD_CLIENT)
    add_subdirectory(client)
endif()

