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

#define PUB_NUM    5000
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

/* 应用层设备操作函数 */
#define DHT11_DEV    "/dev/dht11"
#define AP3216C_DEV  "/dev/ap3216c"
#define ICM20608_DEV "/dev/icm20608"
#define LIGHT_DEV    "/dev/light"
#define FIRE_DEV     "/dev/fire"
#define BEEP_DEV     "/dev/beep_misc"
#define LED_DEV      "/dev/gpioled"
void open_dev(const char* dev);
void close_dev(const char* dev);
void delay_s_dev(const char* dev, int delay);

#endif // __KC_DMS_H__