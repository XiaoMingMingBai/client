cmake_minimum_required(VERSION 3.10)

project(app)

find_package(fmt CONFIG REQUIRED)
find_package(unofficial-curlpp CONFIG REQUIRED)

add_executable(app main.cpp)

target_link_libraries(app PRIVATE fmt::fmt)
target_link_libraries(app PRIVATE unofficial::curlpp::curlpp)