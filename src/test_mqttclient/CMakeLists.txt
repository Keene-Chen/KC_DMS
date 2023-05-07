cmake_minimum_required(VERSION 3.10.0)

project(
  test_mqttclient
  VERSION 1.0.0
  DESCRIPTION "test_mqttclient")

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

# 自定义函数,将生成可执行文件和链接操作合并
function(add_test_file TEST_NAME TEST_FILE)
  add_executable(${TEST_NAME} ${TEST_FILE})
  target_link_libraries(${TEST_NAME} PRIVATE ${LIB_MQTTCLIENT} ${LIB_PTHREAD}
                                             yyjson)
endfunction()

add_executable(
  kc_dms
  kc_dms.c
  dht11_cb.c
  ap3216c_cb.c
  icm20608_cb.c
  light_cb.c
  fire_cb.c
  fan_cb.c
  led_cb.c)
target_link_libraries(kc_dms PRIVATE ${LIB_MQTTCLIENT} ${LIB_PTHREAD} yyjson)
