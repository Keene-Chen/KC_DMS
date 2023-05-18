# KC_DMS
本项目是《基于嵌入式Linux与EMQX云平台的数据监测系统设计与实现》论文的应用层代码，它实现了从嵌入式Linux开发板采集的数据转发到EMQX云平台的功能。

## 目录结构
```
KC_DMS
├── bin
├── clean.sh
├── cmake
│   └── colors.cmake
├── CMakeLists.txt
├── README.md
├── res
│   ├── export.json
│   ├── kc_dms.json
│   ├── kc_dms.schema.json
│   ├── test.ini
│   └── test.log
├── src
│   ├── main
│   ├── test_easylogger
│   ├── test_rxi_ini
│   ├── test_rxi_log
│   └── test_yyjson
└── third_party
    ├── CMakeLists.txt
    ├── easylogger
    ├── mqttclient
    ├── rxi_ini
    ├── rxi_log
    └── yyjson
```

## 第三方库
### INI
- rxi_ini: [GitHub](https://github.com/rxi/ini)

### Log
- rxi_log: [GitHub](https://github.com/rxi/log.c)

- EasyLogger:
  [GitHub](https://github.com/armink/EasyLogger)
  [Doc](https://github.com/armink/EasyLogger/tree/master/docs/zh)

### JSON
- yyjson:
  [GitHub](https://github.com/ibireme/yyjson)
  [Doc](https://ibireme.github.io/yyjson/doc/doxygen/html/)

### MQTT
- paho.mqtt.c:
  [GitHub](https://github.com/eclipse/paho.mqtt.c)
  [Doc](https://eclipse.github.io/paho.mqtt.c/MQTTClient/html/)

- mqttclient:
  [GitHub](https://github.com/jiejieTop/mqttclient)

## Grafana 数据可视化面板
- 实时数据
  ![实时数据](https://cdn.jsdelivr.net/gh/Keene-Chen/KC_DMS/assets/Grafana%20实时数据.png)
- 历史数据
  ![历史数据](https://cdn.jsdelivr.net/gh/Keene-Chen/KC_DMS/assets/Grafana%20历史数据.png)
- EMQX 服务器状态
  ![EMQX 服务器状态](https://cdn.jsdelivr.net/gh/Keene-Chen/KC_DMS/assets/Grafana%20EMQX%20服务器状态.png)