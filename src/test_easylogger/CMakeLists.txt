project(
  test_easylogger
  VERSION 1.0.0
  DESCRIPTION "test_easylogger")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

include_directories(
  ${CMAKE_SOURCE_DIR}/third_party/easylogger/inc
  ${CMAKE_SOURCE_DIR}/third_party/easylogger/plugins/file
  ${CMAKE_SOURCE_DIR}/third_party/easylogger/plugins/flash)

aux_source_directory(${CMAKE_SOURCE_DIR}/third_party/easylogger/port SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/third_party/easylogger/src SRC_LIST)
aux_source_directory(${CMAKE_SOURCE_DIR}/third_party/easylogger/plugins/file
                     SRC_LIST)
link_libraries(pthread)
message(STATUS "${BoldGreen}>>> ${SRC_LIST} >>>${ColourReset}")

add_executable(test_easylogger test_easylogger.c ${SRC_LIST})
add_executable(01_test 01_test.c ${SRC_LIST})
# target_link_libraries(01_test PRIVATE pthread)
