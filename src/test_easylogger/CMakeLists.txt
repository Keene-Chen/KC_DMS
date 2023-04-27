project(
  test_easylogger
  VERSION 1.0.0
  DESCRIPTION "test_easylogger")

# 设置可执行文件、库生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

include_directories(
  ${CMAKE_SOURCE_DIR}/third_party/easylogger/inc
  ${CMAKE_SOURCE_DIR}/third_party/easylogger/plugins/file
  ${CMAKE_SOURCE_DIR}/third_party/easylogger/plugins/flash)

# 自定义函数,将生成可执行文件和链接操作合并
#[[function(add_test_file TEST_NAME TEST_FILE)
  string(APPEND TEST_NAME "_demo")
  add_executable(${TEST_NAME} ${TEST_FILE})
  target_link_libraries(${TEST_NAME} PRIVATE elog)
  endfunction()]]
aux_source_directory(${CMAKE_SOURCE_DIR}/third_party/easylogger/port SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/third_party/easylogger/src SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/third_party/easylogger/plugins/file
                     SRC_LIST)
message(STATUS "${BoldBlue}>>> ${SRC_LIST} >>>${ColourReset}")

add_executable(test_easylogger test_easylogger.c ${SRC_LIST})
add_executable(01_test 01_test.c ${SRC_LIST})
# target_link_libraries(01_test PRIVATE pthread)
