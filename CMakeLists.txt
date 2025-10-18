cmake_minimum_required(VERSION 3.10)

project(BoostAsioExample)

# 设置编译选项，启用优化
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置 Boost 路径
set(BOOST_ROOT "/usr/lib/x86_64-linux-gnu/cmake/Boost-1.71.0" CACHE PATH "Boost library path")

# 查找 Boost 库
find_package(Boost REQUIRED COMPONENTS system serialization)

# 查找 OpenMP 库
find_package(OpenMP REQUIRED)

# 添加可执行文件
add_executable(boost_asio_example main.cpp)

# 链接 Boost 库和 OpenMP 库
target_link_libraries(boost_asio_example Boost::system pthread Boost::serialization OpenMP::OpenMP_CXX)
