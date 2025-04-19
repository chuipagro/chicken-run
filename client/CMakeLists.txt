cmake_minimum_required(VERSION 3.10)
project(Client)

add_executable(client client.cpp)

add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:client> ${CMAKE_SOURCE_DIR}/client_exec
        COMMENT "Copie de l'exécutable client")
