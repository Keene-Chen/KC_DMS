#include "kc_dms.h"
#include "mqttclient.h"
#include "yyjson.h"
#include <unistd.h>

static void topic1_handler(void* client, message_data_t* msg)
{
    (void)client;
    MQTT_LOG_I("%s:%d %s()...\r\ntopic: %s\r\nmessage:%s\r\n", __FILE__, __LINE__, __FUNCTION__,
               msg->topic_name, (char*)msg->message->payload);
}

static void dht11_handler(void* client, message_data_t* msg)
{
    (void)client;
    // MQTT_LOG_I("%s:%d %s()...\r\ntopic: %s\r\nmessage:%s\r\n", __FILE__, __LINE__, __FUNCTION__,
    //            msg->topic_name, (char*)msg->message->payload);
    yyjson_doc* doc =
        yyjson_read((char*)msg->message->payload, strlen((char*)msg->message->payload), 0);

    if (doc) {
        yyjson_val* temp = yyjson_obj_get(doc->root, "temp");
        yyjson_val* humi = yyjson_obj_get(doc->root, "humi");
        if (yyjson_get_real(temp) > 30 || yyjson_get_real(humi) > 70) {
            delay_s_dev(LED_DEV, 1);
        }
    }
    yyjson_doc_free(doc);
}

static void light_handler(void* client, message_data_t* msg)
{
    (void)client;
    // MQTT_LOG_I("%s:%d %s()...\r\ntopic: %s\r\nmessage:%s\r\n", __FILE__, __LINE__, __FUNCTION__,
    //            msg->topic_name, (char*)msg->message->payload);
    yyjson_doc* doc =
        yyjson_read((char*)msg->message->payload, strlen((char*)msg->message->payload), 0);

    if (doc) {
        yyjson_val* light = yyjson_obj_get(doc->root, "light");
        if (yyjson_get_real(light) > 100) {
            delay_s_dev(LED_DEV, 1);
        }
    }
    yyjson_doc_free(doc);
}

int main(void)
{
    int ret;
    printf("\nwelcome to mqttclient test...\n");

    /* 日志初始化 */
    mqtt_log_init();

    /* 连接配置 */
    mqtt_client_t* client = mqtt_lease();
    mqtt_set_port(client, "1883");
    mqtt_set_host(client, "192.168.8.9");
    mqtt_set_client_id(client, "KC_DMS");
    mqtt_set_user_name(client, "chen");
    mqtt_set_password(client, "2555");
    mqtt_set_clean_session(client, 1);
    mqtt_set_write_buf_size(client, 3072);
    mqtt_set_read_buf_size(client, 3072);

    /* 建立MQTT连接 */
    mqtt_connect(client);

    /* 订阅主题 */
    mqtt_subscribe(client, "dht11", QOS0, dht11_handler);
    // mqtt_subscribe(client, "ap3216c", QOS0, topic1_handler);
    // mqtt_subscribe(client, "icm20608", QOS0, topic1_handler);
    mqtt_subscribe(client, "light", QOS0, light_handler);
    // mqtt_subscribe(client, "fire", QOS0, topic1_handler);
    // mqtt_subscribe(client, "fan", QOS0, topic1_handler);
    // mqtt_subscribe(client, "led", QOS0, topic1_handler);

    /* 创建发送线程 */
    pthread_t thread[8];

    mqtt_send_data(thread[0], dht11);
    mqtt_send_data(thread[1], icm20608);
    mqtt_send_data(thread[2], ap3216c);
    mqtt_send_data(thread[3], light);
    mqtt_send_data(thread[4], fire);
    mqtt_send_data(thread[5], beep);
    mqtt_send_data(thread[6], led);

    /* 主线程休眠 */
    while (1) {
        sleep(30);
    }
}