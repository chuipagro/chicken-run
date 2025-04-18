cmake_minimum_required(VERSION 3.10)
project(chickenRun)

# Options pour choisir les builds
option(BUILD_CLIENT "Build the client" ON)
option(BUILD_SERVER "Build the server" ON)

if (BUILD_SERVER)
    add_subdirectory(server)
endif()

if (BUILD_CLIENT)
    add_subdirectory(client)
endif()

include(GNUInstallDirs)

if (BUILD_SERVER)
    add_custom_command(TARGET server POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:server> ${CMAKE_SOURCE_DIR}/server_exec
            COMMENT "Copie de l'exécutable du serveur à la racine")
endif()

if (BUILD_CLIENT)
    add_custom_command(TARGET client POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:client> ${CMAKE_SOURCE_DIR}/client_exec
            COMMENT "Copie de l'exécutable du client à la racine")
endif()
