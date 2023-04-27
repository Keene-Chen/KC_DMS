# Data serialization in C
本项目用于测试常用的在C语言中的数据序列化库，都是最简单的示例文件，详细使用方法以及API请查看第三方库的官方文档。

## 目录结构
```
data_serialization
 ├── bin                // 二进制文件
 ├── cmake              // CMake自定义模块
 ├── cmake-build        // CMake 构建目录
 ├── CMakeLists.txt     // CMake 顶层文件
 ├── clean.sh
 ├── README.md
 ├── res                // 测试数据
 ├── src                // 测试源码目录
 │   ├── test_cJSON
 │   ├── test_easylogger
 │   ├── test_inih
 │   ├── test_libxml2
 │   ├── test_libyaml
 │   ├── test_protobuf-c
 │   ├── test_rxi_ini
 │   ├── test_rxi_log
 │   ├── test_yyjson
 │   └── CMakeLists.txt // CMake 子目录文件
 └── third_party        // 第三方库源码目录
     ├── cJSON
     ├── easylogger
     ├── inih
     ├── rxi_ini
     ├── rxi_log
     ├── yyjson
     └── CMakeLists.txt // CMake 子目录文件
```

## 第三方库
### JSON
- cJSON
  [GitHub](https://github.com/DaveGamble/cJSON)
  [Doc](https://github.com/DaveGamble/cJSON#usage)

- yyjson
  [GitHub](https://github.com/ibireme/yyjson)
  [Doc](https://ibireme.github.io/yyjson/doc/doxygen/html/)

### INI
- inih: [GitHub](https://github.com/benhoyt/inih)

- rxi_ini: [GitHub](https://github.com/rxi/ini)

### Log
- rxi_log: [GitHub](https://github.com/rxi/log.c)

- EasyLogger:
  [GitHub](https://github.com/armink/EasyLogger)
  [Doc](https://github.com/armink/EasyLogger/tree/master/docs/zh)

### YAML
- libyaml:
  [GitHub](https://github.com/yaml/libyaml)
  [Doc](https://pyyaml.org/wiki/LibYAML)

### XML
- libxml2:
  [GitHub](https://github.com/GNOME/libxml2)
  [Doc](https://gitlab.gnome.org/GNOME/libxml2/-/wikis/home)

### Protocol Buffers
- protobuf-c: [GitHub]( https://github.com/protobuf-c/protobuf-c)
