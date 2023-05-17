#ifndef __KC_DMS_H__
#define __KC_DMS_H__

#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mqtt_config.h"
#include "mqtt_log.h"
#include "mqttclient.h"
#include "random.h"
#include "yyjson.h"

#define PUB_NUM    50000
#define SLEEP_TIME 1

/* MQTT TOPIC */
#define DHT11_TOPIC    "dht11"
#define AP3216C_TOPIC  "ap3216c"
#define ICM20608_TOPIC "icm20608"
#define LIGHT_TOPIC    "light"
#define FIRE_TOPIC     "fire"
#define BEEP_TOPIC     "beep"
#define LED_TOPIC      "led"

/* MQTT 发送线程宏函数 */
#define mqtt_send_data(thrid, sense)                                        \
    do {                                                                    \
        ret = pthread_create(&thrid, NULL, sense##_publish_thread, client); \
        if (ret != 0) {                                                     \
            MQTT_LOG_E("create mqtt publish thread fail");                  \
            exit(ret);                                                      \
        }                                                                   \
    } while (0);

/* 发送线程回调函数 */
void* dht11_publish_thread(void* arg);
void* ap3216c_publish_thread(void* arg);
void* icm20608_publish_thread(void* arg);
void* light_publish_thread(void* arg);
void* fire_publish_thread(void* arg);
void* beep_publish_thread(void* arg);
void* led_publish_thread(void* arg);

/* 主题事件函数 */
void dht11_handler(void* client, message_data_t* msg);
void ap3216c_handler(void* client, message_data_t* msg);
void icm20608_handler(void* client, message_data_t* msg);
void light_handler(void* client, message_data_t* msg);
void fire_handler(void* client, message_data_t* msg);
void beep_handler(void* client, message_data_t* msg);
void led_handler(void* client, message_data_t* msg);

/* 应用层设备操作函数 */
#define DHT11_DEV    "/dev/dht11"
#define AP3216C_DEV  "/dev/ap3216c"
#define ICM20608_DEV "/dev/icm20608"
#define LIGHT_DEV    "/dev/light"
#define FIRE_DEV     "/dev/fire"
#define BEEP_DEV     "/dev/beep_misc"
#define RED_LED      "/dev/redled"
#define GREEN_LED    "/dev/greenled"
#define BLUE_LED     "/dev/blueled"
void open_dev(const char* dev);
void close_dev(const char* dev);
void delay_s_dev(const char* dev, int delay);

/* 版本信息 */
#define KCDMS_MAJOR  1
#define KCDMS_MINJOR 0
#define KCDMS_FIX    0
#define prompt(maj, min, fix)                                                                    \
    printf("\n\033[1;35m ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓ \033[1;0m\n");           \
    printf("\033[1;35m ┃ KC_DMS                                     ┃ \033[1;0m\n");             \
    printf("\033[1;35m ┃ Author : KeeneChen                         ┃ \033[1;0m\n");             \
    printf("\033[1;35m ┃ Version: v%d.%d.%d                            ┃ \033[1;0m\n", maj, min, \
           fix);                                                                                 \
    printf("\033[1;35m ┃ GitHub : https://github.com/Keene-Chen     ┃ \033[1;0m\n");             \
    printf("\033[1;35m ┃ Data monitoring system for embedded Linux  ┃ \033[1;0m\n");             \
    printf("\033[1;35m ┃ and EMQX cloud platform                    ┃ \033[1;0m\n");             \
    printf("\033[1;35m ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛ \033[1;0m\n");

#endif // __KC_DMS_H__