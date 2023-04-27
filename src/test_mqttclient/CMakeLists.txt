cmake_minimum_required(VERSION 3.10.0)

project(
  test_mqttclient
  VERSION 1.0.0
  DESCRIPTION "test_mqttclient")

# 查找库位置
find_library(LIB_MQTTCLIENT libmqttclient.so /usr/local/lib REQUIRED)
find_library(LIB_PTHREAD pthread /usr/lib REQUIRED)
if(LIB_PTHREAD)
  message(STATUS ">>> The path to find the libmqttclient is: ${LIB_MQTTCLIENT} >>>")
  message(STATUS ">>> The path to find the pthread is: ${LIB_PTHREAD} >>>")
endif()

# 自定义函数,将生成可执行文件和链接操作合并
function(add_test_file TEST_NAME TEST_FILE)
  add_executable(${TEST_NAME} ${TEST_FILE})
  target_link_libraries(${TEST_NAME} PRIVATE ${LIB_MQTTCLIENT} ${LIB_PTHREAD} yyjson)
endfunction()

add_test_file(client client.c)
