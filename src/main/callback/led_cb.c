#include "kc_dms.h"

/* led传感器数据发送线程回调函数 */
void* led_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    int ret                        = 0;
    unsigned char redled_data[1]   = { 0 };
    unsigned char greenled_data[1] = { 0 };
    unsigned char blueled_data[1]  = { 0 };

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 读取传感器数据 */
    int fd[3] = { 0 };
    fd[0]     = open(RED_LED, O_RDWR);
    fd[1]     = open(GREEN_LED, O_RDWR);
    fd[2]     = open(BLUE_LED, O_RDWR);
    if (fd[0] < 0) {
        printf("file %s open failed!\r\n", RED_LED);
        exit(EXIT_FAILURE);
    }
    else if (fd[1] < 0) {
        printf("file %s open failed!\r\n", GREEN_LED);
        exit(EXIT_FAILURE);
    }
    else if (fd[2] < 0) {
        printf("file %s open failed!\r\n", BLUE_LED);
        exit(EXIT_FAILURE);
    }

    /* 循环读取并发送数据 */
    for (int i = 0; i < PUB_NUM; ++i) {
        ret = read(fd[0], redled_data, sizeof(redled_data));
        ret = read(fd[1], greenled_data, sizeof(greenled_data));
        ret = read(fd[2], blueled_data, sizeof(blueled_data));
        if (ret != 0) {
            printf("read failed!\r\n");
            goto read_fail;
        }

        /* 使用json构建payload */
        yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
        yyjson_mut_val* root = yyjson_mut_obj(doc);
        yyjson_mut_doc_set_root(doc, root);
        yyjson_mut_obj_add_int(doc, root, "redled_status", redled_data[0]);
        yyjson_mut_obj_add_int(doc, root, "greenled_status", greenled_data[0]);
        yyjson_mut_obj_add_int(doc, root, "blueled_status", blueled_data[0]);

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