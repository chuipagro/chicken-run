cmake_minimum_required(VERSION 3.10)
project(Client)

add_executable(client client.cpp)

find_package(SFML 2.5 COMPONENTS graphics window system network REQUIRED)

target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(client sfml-graphics sfml-window sfml-system sfml-network)

add_custom_command(TARGET client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:client> ${CMAKE_SOURCE_DIR}/client_exec
        COMMENT "Copie de l'exécutable client")
