/**
 * @file    : kc_dms.h
 * @author  : KeeneChen
 * @date    : 2023.04.03-10:38:33
 * @details : kc_dms
 */

#ifndef __KC_DMS_H__
#define __KC_DMS_H__

#include <fcntl.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mqtt_config.h"
#include "mqtt_log.h"
#include "mqttclient.h"
#include "msg_data.h"
#include "random.h"
#include "rxi_log.h"
#include "yyjson.h"

/* MQTT TOPIC */
#define TOPIC_PREFIX   "kc_dms/"
#define DHT11_TOPIC    TOPIC_PREFIX "dht11"
#define AP3216C_TOPIC  TOPIC_PREFIX "ap3216c"
#define ICM20608_TOPIC TOPIC_PREFIX "icm20608"
#define LIGHT_TOPIC    TOPIC_PREFIX "light"
#define FIRE_TOPIC     TOPIC_PREFIX "fire"
#define BEEP_TOPIC     TOPIC_PREFIX "beep"
#define LED_TOPIC      TOPIC_PREFIX "led"

// TODO: 发一次一秒,一百万是11天,改成while(1)循环
#define PUB_NUM    1000000
#define SLEEP_TIME 1

/* MQTT 发送线程宏函数 */
#define mqtt_send_data(thrid, sense)                                        \
    do {                                                                    \
        ret = pthread_create(&thrid, NULL, sense##_publish_thread, client); \
        if (ret != 0) {                                                     \
            log_error("create mqtt publish thread fail");                   \
            exit(ret);                                                      \
        }                                                                   \
    } while (0);

/* 初始化日志库 */
#define file_log_init(file_path)         \
    FILE* fp = fopen(file_path, "a+");   \
    if (fp == NULL) {                    \
        printf("failed to open file\n"); \
        return -1;                       \
    }                                    \
    log_add_fp(fp, LOG_INFO);            \
    log_set_quiet(false);

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
#define DEV_PREFIX   "/dev/"
#define DHT11_DEV    DEV_PREFIX "dht11"
#define AP3216C_DEV  DEV_PREFIX "ap3216c"
#define ICM20608_DEV DEV_PREFIX "icm20608"
#define LIGHT_DEV    DEV_PREFIX "light"
#define FIRE_DEV     DEV_PREFIX "fire"
#define BEEP_DEV     DEV_PREFIX "beep_misc"
#define RED_LED      DEV_PREFIX "redled"
#define GREEN_LED    DEV_PREFIX "greenled"
#define BLUE_LED     DEV_PREFIX "blueled"
void open_dev(const char* dev);
void close_dev(const char* dev);
void delay_s_dev(const char* dev, int delay);

/* 常用工具函数 */
#define MAX(a, b)     ((a) > (b) ? (a) : (b))
#define MAX3(a, b, c) MAX(MAX(a, b), c)
#define MIN(a, b)     ((a) < (b) ? (a) : (b))
#define MIN3(a, b, c) MIN(MIN(a, b), c)
#define ABS(a)        ((a) > 0 ? (a) : -(a))
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define PI            3.14159265358979323846

/* 传感器预警值 */
#define TEMP_ALERT  31
#define HUMI_ALERT  70
#define FIRE_ALERT  4000
#define LIGHT_ALERT 300

#endif // __KC_DMS_H__