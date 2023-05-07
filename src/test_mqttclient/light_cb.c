#include "kc_dms.h"

/* light传感器数据发送线程回调函数 */
void* light_publish_thread(void* arg)
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

        // topic: light qos0
        msg.qos     = 0;
        msg.payload = (void*)json;
        mqtt_publish(client, "light", &msg);

        yyjson_mut_doc_free(doc);
        sleep(SLEEP_TIME);
    }

    return NULL;
}