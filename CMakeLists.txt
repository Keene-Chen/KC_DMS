#[[
  file    : KC_DMS/CMakeLists.txt
  author  : KeeneChen
  date    : 2023.03.01-14:26:27
  details : This is the top-level CMakeLists.txt for the project
  github  : https://github.com/Keene-Chen/KC_DMS
]]

cmake_minimum_required(VERSION 3.10.0)

# 设置语言标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CROSS_COMPILE_PATH
    /home/keenechen/IMX6ULL/toolchain/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/
)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

project(
  KC_DMS
  VERSION 1.18.0
  DESCRIPTION "KC_DMS")

# 包括colors.cmake模块,调用其中定义的宏
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(colors)
define_colors()

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
  if("${CMAKE_BINARY_DIR}" MATCHES "build_.*(D|d)ebug$")
    message(STATUS "${Red}>>> Build Debug Type >>>${ColourReset}")
    set(CMAKE_BUILD_TYPE "Debug")
  elseif("${CMAKE_BINARY_DIR}" MATCHES "build_.*(R|r)elWithDebInfo$")
    message(STATUS "${Green}>>> Build RelWithDebInfo Type >>>${ColourReset}")
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
  elseif("${CMAKE_BINARY_DIR}" MATCHES "build_.*(M|m)inSizeRel$")
    message(STATUS "${Blue}>>> Build MinSizeRel Type>>>${ColourReset}")
    set(CMAKE_BUILD_TYPE "MinSizeRel")
  else()
    message(STATUS "${BoldYellow}>>> Build Release Type >>>${ColourReset}")
    set(CMAKE_BUILD_TYPE "Release")
  endif()
endif()

# 导出编译命令配置,供clangd使用
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# 开启测试
enable_testing()

# 添加子目录
add_subdirectory(third_party)
add_subdirectory(src/main)
add_subdirectory(src/test_yyjson)
add_subdirectory(src/test_rxi_ini)
add_subdirectory(src/test_rxi_log)
add_subdirectory(src/test_easylogger)
