project(
  test_rxi_log
  VERSION 1.0.0
  DESCRIPTION "test_rxi_log")

# 设置可执行文件生成位置
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${PROJECT_NAME})

# 自定义函数,将生成可执行文件和链接操作合并
function(add_test_file TEST_NAME TEST_FILE)
  string(APPEND TEST_NAME "_demo")
  add_executable(${TEST_NAME} ${TEST_FILE})
  target_link_libraries(${TEST_NAME} PRIVATE rxi_log)
endfunction()

add_test_file(test_rxi_log test_rxi_log.c)
