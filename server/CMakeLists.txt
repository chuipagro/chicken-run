cmake_minimum_required(VERSION 3.10)
project(Server)
add_executable(server server.cpp
        env.cpp Player.cpp Player.hpp)

add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:server> ${CMAKE_SOURCE_DIR}/server_exec
        COMMENT "Copie de l'exécutable serveur")

