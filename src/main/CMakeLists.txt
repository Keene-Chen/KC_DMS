#[[
  file    : main/CMakeLists.txt
  author  : KeeneChen
  date    : 2023.03.02-19:36:22
  details : A submodule of this project
  github  : https://github.com/Keene-Chen/KC_DMS
]]

cmake_minimum_required(VERSION 3.10.0)

project(
  main
  VERSION 1.18.0
  DESCRIPTION "main")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 查找库位置
include_directories(${CMAKE_SOURCE_DIR}/third_party/mqttclient)
set(LIB_MQTTCLIENT
    ${CMAKE_SOURCE_DIR}/third_party/mqttclient/lib/libmqttclient.so)
find_library(LIB_PTHREAD pthread /usr/lib REQUIRED)
if(LIB_PTHREAD)
  message(STATUS "${BoldMagenta}>>> ${LIB_MQTTCLIENT} >>>${ColourReset}")
  message(STATUS "${BoldMagenta}>>> ${LIB_PTHREAD} >>>${ColourReset}")
endif()

# 添加浮点数支持
set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -march=armv7-a -mfpu=neon -mfloat-abi=hard -lm")

# 设置头文件包含路径
include_directories(./inc)

# 生成可执行文件
aux_source_directory(./callback callback)
aux_source_directory(./event_handler event_handler)
aux_source_directory(./tools tools)
add_executable(kc_dms src/kc_dms.c ${callback} ${event_handler} ${tools})
target_link_libraries(kc_dms PRIVATE ${LIB_MQTTCLIENT} ${LIB_PTHREAD} yyjson
                                     rxi_log)
