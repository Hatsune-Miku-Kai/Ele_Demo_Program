# 指定 CMake 的最低版本
cmake_minimum_required(VERSION 3.10)

# 项目名称和语言
project(MyArmE_LINUX)

# 设置 C++ 标准为 C++11（你可以更改为其他版本，如 C++14, C++17, C++20）
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

# 设置源文件路径
file(GLOB_RECURSE SOURCES "src/*.cpp")  # 获取 src/ 目录下所有的 .cpp 文件
file(GLOB_RECURSE HEADERS "include/*.h") # 获取 include/ 目录下所有的 .h 文件

# 创建一个可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 添加头文件搜索路径
include_directories(${PROJECT_SOURCE_DIR}/include)

# 如果需要链接外部库，使用 target_link_libraries
# 例如，假设你有一个名为 'some_external_lib' 的外部库
# find_package(SomeExternalLib REQUIRED)  # 如果库使用 find_package 查找
# target_link_libraries(${PROJECT_NAME} some_external_lib)

link_directories(~/kevin/MyArmE_LINUX/src/)

# target_link_libraries(${PROJECT_NAME} serialib)

