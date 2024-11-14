# 设置 CMake 最低版本
cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(SimpleCppProject VERSION 1.0)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置默认构建类型为 Debug
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      Debug
      CACHE STRING "Choose the type of build." FORCE)
endif()

# 设置交叉编译器 set(CMAKE_C_COMPILER
# /opt/aarch64--glibc--stable-2020.08-1/bin/aarch64-buildroot-linux-gnu-gcc)
# set(CMAKE_CXX_COMPILER
# /opt/aarch64--glibc--stable-2020.08-1/bin/aarch64-buildroot-linux-gnu-g++)

# 添加可执行文件
file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(SimpleCppProject ${SOURCES})

# 启用调试信息
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

# 指定生成路径
set_target_properties(
  SimpleCppProject
  PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
             ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# 添加生成汇编代码的目标

# 为每个源文件生成汇编文件
foreach(SOURCE_FILE ${SOURCES})
  get_filename_component(FILENAME_WE ${SOURCE_FILE} NAME_WE) # 获取不带扩展名的文件名
  set(ASSEMBLY_FILE "${CMAKE_BINARY_DIR}/src/${FILENAME_WE}.s") # 生成的汇编文件路径

  add_custom_command(
    TARGET SimpleCppProject
    POST_BUILD
    COMMAND ${CMAKE_CXX_COMPILER} -S -o ${ASSEMBLY_FILE} ${SOURCE_FILE} DEPENDS
            ${SOURCE_FILE} # 确保源文件更新时重新生成汇编文件
  )
endforeach()

# C++ FLAGS DEBUG
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE " -O3 -Wall")
set(CMAKE_C_FLAGS_DEBUG "-O0 -Wall -g -ggdb")
set(CMAKE_C_FLAGS_RELEASE " -O3 -Wall")

# 确保 build/src 目录存在
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/src)
