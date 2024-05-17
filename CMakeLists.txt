cmake_minimum_required(VERSION 3.15)

project(client)

find_package(glog)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} glog::glog)