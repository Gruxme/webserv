cmake_minimum_required(VERSION 3.21)
project(webserv)

set(CMAKE_CXX_STANDARD 98)

add_executable(webserv main.cpp sockets.cpp socketsIO.cpp response.cpp response.hpp)
