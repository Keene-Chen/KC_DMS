/**
 * @file    : led_cb.c
 * @author  : KeeneChen
 * @date    : 2023.04.03-10:36:37
 * @details : led_cb
 */

#include "kc_dms.h"

/* led传感器数据发送线程回调函数 */
void* led_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    int ret                  = 0;
    unsigned char buf[3]     = { 0 };
    const char* data_name[3] = { "rled_status", "gled_status", "bled_status" };

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 读取传感器数据 */
    int fd[3] = { 0 };
    fd[0]     = open(RED_LED, O_RDWR);
    fd[1]     = open(GREEN_LED, O_RDWR);
    fd[2]     = open(BLUE_LED, O_RDWR);
    if (fd[0] < 0) {
        log_error("file %s open failed!", RED_LED);
        exit(EXIT_FAILURE);
    }
    else if (fd[1] < 0) {
        log_error("file %s open failed!", GREEN_LED);
        exit(EXIT_FAILURE);
    }
    else if (fd[2] < 0) {
        log_error("file %s open failed!", BLUE_LED);
        exit(EXIT_FAILURE);
    }

    /* 循环读取并发送数据 */
    for (int i = 0; i < PUB_NUM; ++i) {
        ret = read(fd[0], &buf[0], 1);
        ret = read(fd[1], &buf[1], 1);
        ret = read(fd[2], &buf[2], 1);
        if (ret != 0) {
            log_error("read failed!");
            goto read_fail;
        }

        /* 使用json构建payload */
        yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
        yyjson_mut_val* root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);
        for (int i = 0; i < 3; ++i) {
            yyjson_mut_obj_add_int(doc, root, data_name[i], buf[i]);
        }

        // topic: led qos0
        msg.qos     = 0;
        msg.payload = (void*)yyjson_mut_write(doc, 0, NULL);
        mqtt_publish(client, LED_TOPIC, &msg);

        memset(&msg, 0, sizeof(msg));
        yyjson_mut_doc_free(doc);
        sleep(SLEEP_TIME);
    }

read_fail:
    for (int i = 0; i < 3; ++i) {
        close(fd[i]);
    }

    return NULL;
}