cmake_minimum_required(VERSION 3.6)
project(epoll_chatroom_client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES main.cpp error_handler.c error_handler.h necessary.h tcpio.c tcpio.h)
add_executable(epoll_chatroom_client ${SOURCE_FILES})