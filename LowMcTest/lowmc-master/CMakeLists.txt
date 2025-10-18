cmake_minimum_required(VERSION 3.10)

project(LowMcExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加 LowMC 的实现文件
add_library(LowMC LowMC.cpp)

# 编译可执行文件并链接 LowMC 库
add_executable(Lowmc_example test.cpp)
target_link_libraries(Lowmc_example LowMC)