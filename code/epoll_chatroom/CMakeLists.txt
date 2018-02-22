cmake_minimum_required(VERSION 3.6)
project(epoll_chatroom)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES server.cpp necessary.h error_handler.h error_handler.c tcpio.h tcpio.c)
add_executable(epoll_chatroom ${SOURCE_FILES})