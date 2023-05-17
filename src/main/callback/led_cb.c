#include "kc_dms.h"

/* led传感器数据发送线程回调函数 */
void* led_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    unsigned short buf[1] = { 0 };

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 读取传感器数据 */
    int fd = open(RED_LED, O_RDWR);
    if (fd < 0) {
        printf("file %s open failed!\r\n", RED_LED);
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
        if (buf[0] == 1) {
            yyjson_mut_obj_add_int(doc, root, "open", 1);
            yyjson_mut_obj_add_int(doc, root, "close", 0);
        }
        else {
            yyjson_mut_obj_add_int(doc, root, "open", 0);
            yyjson_mut_obj_add_int(doc, root, "close", 1);
        }
        yyjson_mut_obj_add_int(doc, root, "status", buf[0]);

        // 写入字符串
        const char* json = yyjson_mut_write(doc, 0, NULL);

        // topic: led qos0
        msg.qos     = 0;
        msg.payload = (void*)json;
        mqtt_publish(client, LED_TOPIC, &msg);

        yyjson_mut_doc_free(doc);
        sleep(SLEEP_TIME);
    }

read_fail:
    close(fd);

    return NULL;
}