#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mqtt_config.h"
#include "mqttclient/mqtt_log.h"
#include "mqttclient/mqttclient.h"
#include "threadpool.h"
#include "yyjson.h"

static void dht11_handler(void* client, message_data_t* msg)
{
    (void)client;
    MQTT_LOG_I("%s:%d %s()...\ntopic: %s\nmessage:%s\n", __FILE__, __LINE__, __FUNCTION__,
               msg->topic_name, (char*)msg->message->payload);
}

void* dht11_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    char buf[100] = { 0 };
    sprintf(buf, "welcome to mqttclient, this is a publish test...");

    mqtt_list_subscribe_topic(client);
    // msg.payload = (void*)buf;
    // msg.qos     = 0;

    // Create a mutable doc
    yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
    yyjson_mut_val* root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    // Set root["name"] and root["star"]
    yyjson_mut_obj_add_real(doc, root, "temp",
                            random_number_range(1, 30) + (double)random_number_range(1, 30) / 10);
    yyjson_mut_obj_add_real(doc, root, "humi",
                            random_number_range(1, 30) + (double)random_number_range(1, 30) / 10);
    yyjson_mut_obj_add_int(doc, root, "status", 0);

    // To string, minified
    const char* json = yyjson_mut_write(doc, 0, NULL);
    // if (json) {
    //     printf("json: %s\n", json); // {"name":"Mash","star":4,"hits":[2,2,1,3]}
    //     free((void*)json);
    // }

    for (int i = 0; i < 10; ++i) {
        msg.payload = (void*)json;
        mqtt_publish(client, "dht11", &msg);

        // mqtt_publish(client, "mpu6050", &msg[1]);
        // mqtt_publish(client, "mq-7", &msg[2]);
        // mqtt_publish(client, "fire", &msg[3]);
        // mqtt_publish(client, "fan", &msg[4]);
        // mqtt_publish(client, "led", &msg[5]);

        // sleep(1);
    }
    // Free the doc
    yyjson_mut_doc_free(doc);
    return NULL;
}

void* mpu6050_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    char buf[100] = { 0 };
    sprintf(buf, "welcome to mqttclient, this is a publish test...");

    mqtt_list_subscribe_topic(client);
    // msg.payload = (void*)buf;
    // msg.qos     = 0;

    // Create a mutable doc
    yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
    yyjson_mut_val* root = yyjson_mut_obj(doc);
    yyjson_mut_doc_set_root(doc, root);

    // Set root["name"] and root["star"]
    yyjson_mut_obj_add_real(doc, root, "temp",
                            random_number_range(1, 30) + (double)random_number_range(1, 30) / 10);
    yyjson_mut_obj_add_real(doc, root, "humi",
                            random_number_range(1, 30) + (double)random_number_range(1, 30) / 10);
    yyjson_mut_obj_add_int(doc, root, "status", 0);

    // To string, minified
    const char* json = yyjson_mut_write(doc, 0, NULL);
    // if (json) {
    //     printf("json: %s\n", json); // {"name":"Mash","star":4,"hits":[2,2,1,3]}
    //     free((void*)json);
    // }

    for (int i = 0; i < 10; ++i) {
        msg.payload = (void*)json;
        mqtt_publish(client, "mpu6050", &msg);
    }
    // Free the doc
    yyjson_mut_doc_free(doc);
    return NULL;
}

int main(void)
{
    int res;
    pthread_t thread1, thread2;
    mqtt_client_t* client = NULL;
    char client_id[32];
    char user_name[32];
    char password[32];

    printf("\nwelcome to mqttclient test...\n");
    strcpy(client_id, random_string(10));
    strcpy(user_name, random_string(10));
    strcpy(password, random_string(10));

    mqtt_log_init();

    client = mqtt_lease();

    mqtt_set_port(client, "1883");
    mqtt_set_host(client, "192.168.8.9");
    mqtt_set_client_id(client, client_id);
    mqtt_set_user_name(client, user_name);
    mqtt_set_password(client, password);
    mqtt_set_clean_session(client, 1);

    mqtt_connect(client);

    mqtt_subscribe(client, "dht11", QOS0, dht11_handler);
    mqtt_subscribe(client, "mpu6050", QOS0, NULL);
    // mqtt_subscribe(client, "topic2", QOS1, NULL);
    // mqtt_subscribe(client, "topic3", QOS2, NULL);

    /*  res = pthread_create(&thread1, NULL, mqtt_publish_thread, client);
     while (1) {
         sleep(100);
     } */

    // 创建线程池
    ThreadPool* pool = threadpool_create(2, 5, 3);
    res              = threadpool_add(pool, dht11_publish_thread, client);
    res              = threadpool_add(pool, mpu6050_publish_thread, client);
    if (res == 0) {
        MQTT_LOG_E("create mqtt publish thread fail");
        exit(res);
    }

    // 让主线程睡眠30s,防止子线程未执行完就关闭线程池
    sleep(45);

    threadpool_destroy(pool);
}