#include "kc_dms.h"

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
        sleep(SLEEP_TIME);
    }

read_fail:
    close(fd);

    return NULL;
}