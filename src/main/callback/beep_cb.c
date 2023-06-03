/**
 * @file    : beep_cb.c
 * @author  : KeeneChen
 * @date    : 2023.04.03-11:34:50
 * @details : beep_cb
 */

#include "kc_dms.h"

/* beep传感器数据发送线程回调函数 */
void* beep_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    int ret               = 0;
    unsigned short buf[1] = { 0 };

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 读取传感器数据 */
    int fd = open(BEEP_DEV, O_RDWR);
    if (fd < 0) {
        log_error("file %s open failed!", BEEP_DEV);
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
        if (buf[0] == 1) {
            yyjson_mut_obj_add_int(doc, root, "open", 1);
            yyjson_mut_obj_add_int(doc, root, "close", 0);
        }
        else {
            yyjson_mut_obj_add_int(doc, root, "open", 0);
            yyjson_mut_obj_add_int(doc, root, "close", 1);
        }
        yyjson_mut_obj_add_int(doc, root, "status", buf[0]);

        // topic: beep qos0
        msg.qos     = 0;
        msg.payload = (void*)yyjson_mut_write(doc, 0, NULL);
        mqtt_publish(client, BEEP_TOPIC, &msg);

        memset(&msg, 0, sizeof(msg));
        yyjson_mut_doc_free(doc);
        sleep(SLEEP_TIME);
    }

read_fail:
    close(fd);

    return NULL;
}