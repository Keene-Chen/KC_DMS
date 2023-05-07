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

#define PUB_NUM    1000
#define SLEEP_TIME 2

#define mqtt_send_data(thrid, sense)                                        \
    do {                                                                    \
        ret = pthread_create(&thrid, NULL, sense##_publish_thread, client); \
        if (ret != 0) {                                                     \
            MQTT_LOG_E("create mqtt publish thread fail");                  \
            exit(ret);                                                      \
        }                                                                   \
    } while (0);

void* dht11_publish_thread(void* arg);
void* ap3216c_publish_thread(void* arg);
void* icm20608_publish_thread(void* arg);
void* light_publish_thread(void* arg);
void* fire_publish_thread(void* arg);
void* fan_publish_thread(void* arg);
void* led_publish_thread(void* arg);

#endif // __KC_DMS_H__