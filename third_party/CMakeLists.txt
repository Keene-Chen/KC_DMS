#[[
  file    : third_party/CMakeLists.txt
  author  : KeeneChen
  date    : 2023.04.27-15:53:47
  details : This is the CMakeLists.txt for submodules of third-party libraries
]]

project(
  test_3rd
  VERSION 1.0.0
  DESCRIPTION "test_3rd")

# Custom functions are used to compile third-party libraries
function(kc_add_library LIB_NAME LIB_TYPE LIB_PATH)
  add_library(${LIB_NAME} SHARED "")
  target_sources(
    ${LIB_NAME}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/${LIB_PATH}.c
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${LIB_PATH}.h)
  target_include_directories(${LIB_NAME}
                             PUBLIC ${CMAKE_CURRENT_LIST_DIR}/${LIB_NAME})
  message(STATUS "${BoldMagenta}>>> ${LIB_NAME}.so >>>${ColourReset}")
endfunction(kc_add_library)

#[[
  cJSON: https://github.com/DaveGamble/cJSON
  Doc: https://github.com/DaveGamble/cJSON#usage
]]
kc_add_library(cJSON SHARED cJSON/cJSON)
kc_add_library(cJSON_Utils SHARED cJSON/cJSON_Utils)

#[[
  yyjson: https://github.com/ibireme/yyjson
  Doc: https://ibireme.github.io/yyjson/doc/doxygen/html/
]]
kc_add_library(yyjson SHARED yyjson/yyjson)

#[[ rxi_ini: https://github.com/rxi/ini ]]
kc_add_library(rxi_ini SHARED rxi_ini/rxi_ini)

#[[ rxi_log: https://github.com/rxi/log.c ]]
kc_add_library(rxi_log SHARED rxi_log/rxi_log)
target_compile_definitions(rxi_log PRIVATE -DLOG_USE_COLOR)

#[[
  EasyLogger: https://github.com/armink/EasyLogger
  Doc: https://github.com/armink/EasyLogger/tree/master/docs/zh
  Note: This library needs to be configured by yourself
]]
message(
  STATUS
    "${BoldBlue}>>> see also ${CMAKE_SOURCE_DIR}/src/test_easylogger >>>${ColourReset}"
)

#[[
  mqttclient: https://github.com/armink/EasyLogger
  Doc: https://github.com/armink/EasyLogger/tree/master/docs/zh
  Note: This library needs to be configured by yourself
]]
