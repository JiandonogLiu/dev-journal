# 设置 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(SimpleCppProject VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置交叉编译器 set(CMAKE_C_COMPILER
# /opt/aarch64--glibc--stable-2020.08-1/bin/aarch64-buildroot-linux-gnu-gcc)
# set(CMAKE_CXX_COMPILER
# /opt/aarch64--glibc--stable-2020.08-1/bin/aarch64-buildroot-linux-gnu-g++)

# 添加可执行文件
add_executable(SimpleCppProject src/main.cpp)

# 启用调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 指定生成路径
set_target_properties(
  SimpleCppProject
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
             ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# 添加生成汇编代码的目标
add_custom_command(
  TARGET SimpleCppProject
  POST_BUILD
  COMMAND
    ${CMAKE_CXX_COMPILER} -S -o ${CMAKE_BINARY_DIR}/src/main.s
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp DEPENDS
    SimpleCppProject # 确保在 SimpleCppProject 构建后生成汇编文件
)

# 确保 build/src 目录存在
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src)
