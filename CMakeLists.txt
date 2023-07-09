cmake_minimum_required(VERSION 3.10)
project(TCPServer)

set(CMAKE_CXX_STANDARD 11)

add_executable(server
        server.cpp
        client.cpp
        main.cpp)

target_link_libraries(server
        pthread)
