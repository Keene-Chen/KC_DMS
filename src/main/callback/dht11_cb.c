/**
 * @file    : dht11_cb.c
 * @author  : KeeneChen
 * @date    : 2023.04.03-10:35:17
 * @details : dht11_cb
 */

#include "kc_dms.h"

/* dht11传感器数据发送线程回调函数 */
void* dht11_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    int ret              = 0;
    unsigned char buf[5] = { 0 };

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 读取传感器数据 */
    int fd = open(DHT11_DEV, O_RDWR);
    if (fd < 0) {
        log_error("file %s open failed!", DHT11_DEV);
        exit(EXIT_FAILURE);
    }

    /* 循环读取并发送数据 */
    for (int i = 0; i < PUB_NUM; ++i) {
        ret = read(fd, buf, sizeof(buf));
        if (ret != 0) {
            log_error("read failed!");
            goto read_fail;
        }

        /* 使用json构建payload */
        yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
        yyjson_mut_val* root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);
        yyjson_mut_obj_add_real(doc, root, "temp", buf[2] - 2 + ((float)buf[3] / 10));
        yyjson_mut_obj_add_real(doc, root, "humi", buf[0] + 15);
        yyjson_mut_obj_add_int(doc, root, "status", 1);

        // topic: dht11 qos0
        msg.qos     = 0;
        msg.payload = (void*)yyjson_mut_write(doc, 0, NULL);
        mqtt_publish(client, DHT11_TOPIC, &msg);

        memset(&msg, 0, sizeof(msg));
        yyjson_mut_doc_free(doc);
        sleep(SLEEP_TIME);
    }

read_fail:
    close(fd);

    return NULL;
}