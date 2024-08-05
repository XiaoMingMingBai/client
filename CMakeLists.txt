cmake_minimum_required(VERSION 3.10)

project(app)

find_package(fmt CONFIG REQUIRED)
find_package(unofficial-curlpp CONFIG REQUIRED)

# 获取 src 目录及其所有子目录中的所有源文件
file(GLOB_RECURSE ALL_SOURCES "src/*.cpp" "src/*.hpp")

# 添加可执行文件，并包含所有源文件
add_executable(app ${ALL_SOURCES})

target_link_libraries(app PRIVATE fmt::fmt)
target_link_libraries(app PRIVATE unofficial::curlpp::curlpp)