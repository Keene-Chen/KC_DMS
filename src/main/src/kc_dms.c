/**
 * @file    : kc_dms.c
 * @author  : KeeneChen
 * @date    : 2023.04.03-10:39:17
 * @details : 每个发送线程对应一个传感器主题
 */

#include "kc_dms.h"
#include "mqttclient.h"
#include "yyjson.h"
#include <unistd.h>

int main(void)
{
    int ret;
    prompt(KCDMS_MAJOR, KCDMS_MINJOR, KCDMS_FIX);

    /* 日志初始化 */
    mqtt_log_init();
    file_log_init("./kc_dms.log");

    /* 连接配置 */
    mqtt_client_t* client = mqtt_lease();
    mqtt_set_port(client, "1883");
    mqtt_set_host(client, "192.168.8.9");
    mqtt_set_client_id(client, "KC_DMS");
    mqtt_set_user_name(client, "chen");
    mqtt_set_password(client, "2555");
    mqtt_set_clean_session(client, 1);
    mqtt_set_write_buf_size(client, 1024);
    mqtt_set_read_buf_size(client, 1024);

    /* 建立MQTT连接 */
    mqtt_connect(client);

    /* 订阅主题 */
    mqtt_subscribe(client, DHT11_TOPIC, QOS0, dht11_handler);
    mqtt_subscribe(client, AP3216C_TOPIC, QOS0, ap3216c_handler);
    mqtt_subscribe(client, ICM20608_TOPIC, QOS0, icm20608_handler);
    mqtt_subscribe(client, LIGHT_TOPIC, QOS0, light_handler);
    mqtt_subscribe(client, FIRE_TOPIC, QOS0, fire_handler);
    mqtt_subscribe(client, BEEP_TOPIC, QOS0, beep_handler);
    mqtt_subscribe(client, LED_TOPIC, QOS0, led_handler);

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