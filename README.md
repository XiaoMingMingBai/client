# Client

## 编译

1. 安装[Conan](https://conan.io/downloads.html)和[CMake](https://cmake.org/download/)。
2. 编辑配置文件

```shell
cd Client
# 检索并生成默认配置
conan profile detect --force 
# 查看配置路径
conan profile path default
# conan安装包并构建项目
conan install . --build=missing

cd build
# 生成构建配置makefile文件
cmake .. -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
# 开始构建
cmake --build .
# 执行应用程序
./compressor
```

## 交叉编译

```shell
cd Client
conan install . --build=missing -pr:b=default -pr:h=./profiles/raspberry
cd build
source Release/generators/conanbuild.sh 
cmake .. -DCMAKE_TOOLCHAIN_FILE=Release/generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
source Release/generators/deactivate_conanbuild.sh
# 检查
file compressor
```