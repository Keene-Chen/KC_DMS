/**
 * @file    : icm20608_cb.c
 * @author  : KeeneChen
 * @date    : 2023.04.03-10:35:43
 * @details : icm20608_cb
 */

#include "kc_dms.h"

/* icm20608传感器数据发送线程回调函数 */
void* icm20608_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    int ret                  = 0;
    int buf[7]               = { 0 };
    const char* data_name[8] = { "accel_x", "accel_y", "accel_z", "gyro_x",
                                 "gyro_y",  "gyro_z",  "temp",    "status" };
    const char* FORMAT       = "%.4f";

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 读取传感器数据 */
    int fd = open(ICM20608_DEV, O_RDWR);
    if (fd < 0) {
        log_error("file %s open failed!", ICM20608_DEV);
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

        /**
         * 丑陋的代码一
         * yyjson_mut_obj_add_real(doc, root, "accel_x", (float)(buf[0]) / 16.4);
         * yyjson_mut_obj_add_real(doc, root, "accel_y", (float)(buf[1]) / 16.4);
         * yyjson_mut_obj_add_real(doc, root, "accel_z", (float)(buf[2]) / 16.4);
         * yyjson_mut_obj_add_real(doc, root, "gyro_x", (float)(buf[3]) / 2048);
         * yyjson_mut_obj_add_real(doc, root, "gyro_y", (float)(buf[4]) / 2048);
         * yyjson_mut_obj_add_real(doc, root, "gyro_z", (float)(buf[5]) / 2048);
         * yyjson_mut_obj_add_real(doc, root, "temp", ((float)(buf[6]) - 25) / 326.8 + 25);
         * yyjson_mut_obj_add_real(doc, root, "status", 1);
         *
         * 丑陋的代码二
         * sprintf(str[1], "%.2f", (float)(buf[1]) / 16.4);
         * sprintf(str[2], "%.2f", (float)(buf[2]) / 16.4);
         * sprintf(str[3], "%.2f", (float)(buf[3]) / 2048);
         * sprintf(str[4], "%.2f", (float)(buf[4]) / 2048);
         * sprintf(str[5], "%.2f", (float)(buf[5]) / 2048);
         * sprintf(str[6], "%.2f", ((float)(buf[6]) - 25) / 326.8 + 25);
         * yyjson_mut_obj_add_real(doc, root, "accel_x", atof(str[0]));
         * yyjson_mut_obj_add_real(doc, root, "accel_y", atof(str[1]));
         * yyjson_mut_obj_add_real(doc, root, "accel_z", atof(str[2]));
         * yyjson_mut_obj_add_real(doc, root, "gyro_x", atof(str[3]));
         * yyjson_mut_obj_add_real(doc, root, "gyro_y", atof(str[4]));
         * yyjson_mut_obj_add_real(doc, root, "gyro_z", atof(str[5]));
         * yyjson_mut_obj_add_real(doc, root, "temp", atof(str[6]));
         * yyjson_mut_obj_add_real(doc, root, "status", 1);
         *
         * 丑陋的代码三、只想保留个两位小数太抽象了
         */
        for (int i = 0; i < 8; ++i) {
            if (i < 3) {
                char str[10] = { 0 };
                sprintf(str, FORMAT, (float)(buf[i]) / 16.4);
                yyjson_mut_obj_add_real(doc, root, data_name[i], atof(str));
            }
            else if (i < 6) {
                char str[10] = { 0 };
                sprintf(str, FORMAT, (float)(buf[i]) / 2048);
                yyjson_mut_obj_add_real(doc, root, data_name[i], atof(str));
            }
            else if (i < 7) {
                char str[10] = { 0 };
                sprintf(str, FORMAT, ((float)(buf[6]) - 25) / 326.8 + 25);
                yyjson_mut_obj_add_real(doc, root, data_name[i], atof(str));
            }
            else {
                yyjson_mut_obj_add_int(doc, root, data_name[i], 1);
            }
        }

        // topic: icm20608 qos0
        msg.qos     = 0;
        msg.payload = (void*)yyjson_mut_write(doc, 0, NULL);
        mqtt_publish(client, ICM20608_TOPIC, &msg);

        memset(&msg, 0, sizeof(msg));
        yyjson_mut_doc_free(doc);
        sleep(SLEEP_TIME);
    }

read_fail:
    close(fd);

    return NULL;
}