#include "kc_dms.h"

/* icm20608传感器数据发送线程回调函数 */
void* icm20608_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    int buf[7] = { 0 };
    int gyro_x_adc, gyro_y_adc, gyro_z_adc;
    int accel_x_adc, accel_y_adc, accel_z_adc;
    int temp_adc;

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 读取传感器数据 */
    int fd = open(ICM20608_DEV, O_RDWR);
    if (fd < 0) {
        printf("file %s open failed!\r\n", ICM20608_DEV);
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
        gyro_x_adc  = buf[0];
        gyro_y_adc  = buf[1];
        gyro_z_adc  = buf[2];
        accel_x_adc = buf[3];
        accel_y_adc = buf[4];
        accel_z_adc = buf[5];
        temp_adc    = buf[6];
        yyjson_mut_obj_add_real(doc, root, "accel.x", (float)(gyro_x_adc) / 16.4);
        yyjson_mut_obj_add_real(doc, root, "accel.y", (float)(gyro_y_adc) / 16.4);
        yyjson_mut_obj_add_real(doc, root, "accel.z", (float)(gyro_z_adc) / 16.4);
        yyjson_mut_obj_add_real(doc, root, "gyro.x", (float)(accel_x_adc) / 2048);
        yyjson_mut_obj_add_real(doc, root, "gyro.y", (float)(accel_y_adc) / 2048);
        yyjson_mut_obj_add_real(doc, root, "gyro.z", (float)(accel_z_adc) / 2048);
        yyjson_mut_obj_add_real(doc, root, "temp", ((float)(temp_adc)-25) / 326.8 + 25);
        yyjson_mut_obj_add_real(doc, root, "status", 0);

        // 写入字符串
        const char* json = yyjson_mut_write(doc, 0, NULL);

        // topic: icm20608 qos0
        msg.qos     = 0;
        msg.payload = (void*)json;
        mqtt_publish(client, ICM20608_TOPIC, &msg);

        memset(&buf, 0, sizeof(buf));
        yyjson_mut_doc_free(doc);
        sleep(SLEEP_TIME);
    }

read_fail:
    close(fd);

    return NULL;
}