#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mqtt_config.h"
#include "mqtt_log.h"
#include "mqttclient.h"
#include "random.h"
#include "yyjson.h"

#define PUB_NUM 10

#define mqtt_send_data(thrid, sense)                                        \
    do {                                                                    \
        ret = pthread_create(&thrid, NULL, sense##_publish_thread, client); \
        if (ret != 0) {                                                     \
            MQTT_LOG_E("create mqtt publish thread fail");                  \
            exit(ret);                                                      \
        }                                                                   \
    } while (0);

static void topic1_handler(void* client, message_data_t* msg)
{
    (void)client;
    MQTT_LOG_I("%s:%d %s()...\ntopic: %s\nmessage:%s", __FILE__, __LINE__, __FUNCTION__,
               msg->topic_name, (char*)msg->message->payload);
}

/* dht11传感器数据发送线程回调函数 */
void* dht11_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    unsigned short buf[3] = { 0 };

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 读取传感器数据 */
    // int fd = open("/dev/ap3216c", O_RDWR);
    // if (fd < 0) {
    //     printf("file %s open failed!\r\n", "/dev/ap3216c");
    //     exit(EXIT_FAILURE);
    // }

    /* 循环读取并发送数据 */
    for (int i = 0; i < PUB_NUM; ++i) {
        // int ret = read(fd, buf, sizeof(buf));
        // if (ret != 0) {
        //     printf("read failed!\r\n");
        //     goto read_fail;
        // }

        /* 使用json构建payload */
        yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
        yyjson_mut_val* root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);

        // Set root["name"] and root["star"]
        yyjson_mut_obj_add_real(doc, root, "temp",
                                random_number_range(0, 50)
                                    + (double)random_number_range(0, 99) / 10);
        yyjson_mut_obj_add_real(doc, root, "humi",
                                random_number_range(0, 80)
                                    + (double)random_number_range(0, 99) / 10);
        yyjson_mut_obj_add_int(doc, root, "status", 0);

        // 写入字符串
        const char* json = yyjson_mut_write(doc, 0, NULL);

        // topic: dht11 qos0
        msg.qos     = 0;
        msg.payload = (void*)json;
        mqtt_publish(client, "dht11", &msg);

        yyjson_mut_doc_free(doc);
        sleep(2);
    }

    // read_fail:
    //     close(fd);

    return NULL;
}

/* ap3216c传感器数据发送线程回调函数 */
void* ap3216c_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    unsigned short buf[3] = { 0 };

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 读取传感器数据 */
    int fd = open("/dev/ap3216c", O_RDWR);
    if (fd < 0) {
        printf("file %s open failed!\r\n", "/dev/ap3216c");
        exit(EXIT_FAILURE);
    }

    /* 循环读取并发送数据 */
    for (int i = 0; i < PUB_NUM; ++i) {
        int ret = read(fd, buf, sizeof(buf));
        if (ret != 0) {
            printf("read failed!\r\n");
            goto read_fail;
        }

        /* 使用json构建payload */
        yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
        yyjson_mut_val* root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);

        // Set root["name"] and root["star"]
        yyjson_mut_obj_add_int(doc, root, "ir", buf[0]);
        yyjson_mut_obj_add_int(doc, root, "als", buf[1]);
        yyjson_mut_obj_add_int(doc, root, "ps", buf[2]);
        yyjson_mut_obj_add_int(doc, root, "status", 0);

        // 写入字符串
        const char* json = yyjson_mut_write(doc, 0, NULL);

        // topic: ap3216c qos0
        msg.qos     = 0;
        msg.payload = (void*)json;
        mqtt_publish(client, "ap3216c", &msg);

        yyjson_mut_doc_free(doc);
        sleep(1);
    }

read_fail:
    close(fd);

    return NULL;
}

/* fire传感器数据发送线程回调函数 */
void* fire_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    unsigned short buf[3] = { 0 };

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 循环读取并发送数据 */
    for (int i = 0; i < PUB_NUM; ++i) {
        /* 使用json构建payload */
        yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
        yyjson_mut_val* root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);

        // Set root["name"] and root["star"]
        if (random_number_range(0, 2)) {
            yyjson_mut_obj_add_int(doc, root, "open", 1);
            yyjson_mut_obj_add_int(doc, root, "close", 0);
        }
        else {
            yyjson_mut_obj_add_int(doc, root, "open", 0);
            yyjson_mut_obj_add_int(doc, root, "close", 1);
        }
        yyjson_mut_obj_add_int(doc, root, "status", random_number_range(0, 2));

        // 写入字符串
        const char* json = yyjson_mut_write(doc, 0, NULL);

        // topic: fire qos0
        msg.qos     = 0;
        msg.payload = (void*)json;
        mqtt_publish(client, "fire", &msg);

        yyjson_mut_doc_free(doc);
        sleep(1);
    }

    return NULL;
}

/* fan传感器数据发送线程回调函数 */
void* fan_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    unsigned short buf[3] = { 0 };

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 循环读取并发送数据 */
    for (int i = 0; i < PUB_NUM; ++i) {
        /* 使用json构建payload */
        yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
        yyjson_mut_val* root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);

        // Set root["name"] and root["star"]
        if (random_number_range(0, 2)) {
            yyjson_mut_obj_add_int(doc, root, "open", 1);
            yyjson_mut_obj_add_int(doc, root, "close", 0);
        }
        else {
            yyjson_mut_obj_add_int(doc, root, "open", 0);
            yyjson_mut_obj_add_int(doc, root, "close", 1);
        }
        yyjson_mut_obj_add_int(doc, root, "status", random_number_range(0, 1));

        // 写入字符串
        const char* json = yyjson_mut_write(doc, 0, NULL);

        // topic: fan qos0
        msg.qos     = 0;
        msg.payload = (void*)json;
        mqtt_publish(client, "fan", &msg);

        yyjson_mut_doc_free(doc);
        sleep(1);
    }

    return NULL;
}

/* led传感器数据发送线程回调函数 */
void* led_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    unsigned short buf[3] = { 0 };

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 循环读取并发送数据 */
    for (int i = 0; i < PUB_NUM; ++i) {
        /* 使用json构建payload */
        yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
        yyjson_mut_val* root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);

        // Set root["name"] and root["star"]
        if (random_number_range(0, 2)) {
            yyjson_mut_obj_add_int(doc, root, "open", 1);
            yyjson_mut_obj_add_int(doc, root, "close", 0);
        }
        else {
            yyjson_mut_obj_add_int(doc, root, "open", 0);
            yyjson_mut_obj_add_int(doc, root, "close", 1);
        }
        yyjson_mut_obj_add_int(doc, root, "status", random_number_range(0, 1));

        // 写入字符串
        const char* json = yyjson_mut_write(doc, 0, NULL);

        // topic: led qos0
        msg.qos     = 0;
        msg.payload = (void*)json;
        mqtt_publish(client, "led", &msg);

        yyjson_mut_doc_free(doc);
        sleep(1);
    }

    return NULL;
}

int main(void)
{
    int ret;
    mqtt_client_t* client = NULL;
    char client_id[32]    = "KC_DMS";
    char user_name[32]    = "chen";
    char password[32]     = "2555";

    printf("\nwelcome to mqttclient test...\n");

    /* 日志初始化 */
    mqtt_log_init();

    /* 连接配置 */
    client = mqtt_lease();
    mqtt_set_port(client, "1883");
    mqtt_set_host(client, "192.168.8.9");
    mqtt_set_client_id(client, client_id);
    mqtt_set_user_name(client, user_name);
    mqtt_set_password(client, password);
    mqtt_set_clean_session(client, 1);

    /* 建立MQTT连接 */
    mqtt_connect(client);

    /* 订阅主题 */
    mqtt_subscribe(client, "ap3216c", QOS0, topic1_handler);
    mqtt_subscribe(client, "dht11", QOS0, topic1_handler);
    mqtt_subscribe(client, "fire", QOS0, topic1_handler);
    mqtt_subscribe(client, "fan", QOS0, topic1_handler);
    mqtt_subscribe(client, "led", QOS0, topic1_handler);

    /* 创建发送线程 */
    pthread_t thread[5];

    // mqtt_send_data(thread[0], ap3216c);
    mqtt_send_data(thread[1], dht11);
    // mqtt_send_data(thread[2], fire);
    // mqtt_send_data(thread[3], fan);
    // mqtt_send_data(thread[4], led);

    // pthread_exit(&thread[0]);
    /* 主线程休眠 */
    while (1) {
        sleep(30);
    }
}