cmake_minimum_required(VERSION 3.22)
project(chat_room)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(chat_room "server.cc" "socket.cc")