/**
 * @file    : fire_cb.c
 * @author  : KeeneChen
 * @date    : 2023.04.03-10:35:29
 * @details : fire_cb
 */

#include "kc_dms.h"

/* 字符串转数字，将浮点小数字符串转换为浮点数数值 */
#define SENSOR_FLOAT_DATA_GET(ret, index, str, member)   \
    ret         = file_data_read(file_path[index], str); \
    dev->member = atof(str);

/* 字符串转数字，将整数字符串转换为整数数值 */
#define SENSOR_INT_DATA_GET(ret, index, str, member)     \
    ret         = file_data_read(file_path[index], str); \
    dev->member = atoi(str);

/* adc iio框架对应的文件路径 */
static char* file_path[] = {
    "/sys/bus/iio/devices/iio:device0/in_voltage_scale",
    "/sys/bus/iio/devices/iio:device0/in_voltage4_raw",
};

/* 文件路径索引，要和file_path里面的文件顺序对应 */
enum path_index {
    IN_VOLTAGE_SCALE = 0,
    IN_VOLTAGE_RAW,
};

/*
 * ADC数据设备结构体
 */
struct adc_dev {
    int raw;
    float scale;
    float act;
};

struct adc_dev imx6ulladc;

/*
 * @description			: 读取指定文件内容
 * @param - filename 	: 要读取的文件路径
 * @param - str 		: 读取到的文件字符串
 * @return 				: 0 成功;其他 失败
 */
static int file_data_read(char* filename, char* str)
{
    int ret = 0;
    FILE* data_stream;

    data_stream = fopen(filename, "r"); /* 只读打开 */
    if (data_stream == NULL) {
        log_error("can't open file %s", filename);
        return -1;
    }

    ret = fscanf(data_stream, "%s", str);
    if (!ret) {
        log_error("file read error!");
    }
    else if (ret == EOF) {
        /* 读到文件末尾的话将文件指针重新调整到文件头 */
        fseek(data_stream, 0, SEEK_SET);
    }
    fclose(data_stream); /* 关闭文件 */
    return 0;
}

/*
 * @description	: 获取ADC数据
 * @param - dev : 设备结构体
 * @return 		: 0 成功;其他 失败
 */
static int adc_read(struct adc_dev* dev)
{
    int ret = 0;
    char str[50];

    SENSOR_FLOAT_DATA_GET(ret, IN_VOLTAGE_SCALE, str, scale);
    SENSOR_INT_DATA_GET(ret, IN_VOLTAGE_RAW, str, raw);

    /* 转换得到实际电压值mV */
    dev->act = (dev->scale * dev->raw) / 1000.f;
    return ret;
}

/* fire传感器数据发送线程回调函数 */
void* fire_publish_thread(void* arg)
{
    mqtt_client_t* client = (mqtt_client_t*)arg;
    mqtt_message_t msg;
    memset(&msg, 0, sizeof(msg));
    unsigned short buf[3] = { 0 };
    int ret = 0, count = 0, avg = 0;
    const char* data_name[3] = { "raw", "voltage", "status" };
    const char* FORMAT       = "%.2f";

    /* 监听订阅主题 */
    mqtt_list_subscribe_topic(client);

    /* 循环读取并发送数据 */
    for (int i = 0; i < PUB_NUM; ++i) {
        for (int i = 0; i < 20; i++) {
            ret = adc_read(&imx6ulladc);
            count += imx6ulladc.raw;
        }
        avg = count / 20;

        /* 数据读取成功 */
        if (ret == 0) {
            /* 使用json构建payload */
            yyjson_mut_doc* doc  = yyjson_mut_doc_new(NULL);
            yyjson_mut_val* root = yyjson_mut_obj(doc);
            yyjson_mut_doc_set_root(doc, root);
            yyjson_mut_obj_add_int(doc, root, data_name[0], avg);
            char str[10] = { 0 };
            sprintf(str, FORMAT, imx6ulladc.act / 100);
            yyjson_mut_obj_add_real(doc, root, data_name[1], atof(str));
            if (avg < 4000) {
                yyjson_mut_obj_add_int(doc, root, data_name[2], 1);
            }
            else {
                yyjson_mut_obj_add_int(doc, root, data_name[2], 0);
            }

            // topic: fire qos0
            msg.qos     = 0;
            msg.payload = (void*)yyjson_mut_write(doc, 0, NULL);
            mqtt_publish(client, FIRE_TOPIC, &msg);

            memset(&msg, 0, sizeof(msg));
            yyjson_mut_doc_free(doc);
            sleep(SLEEP_TIME);
        }
        count = 0;
    }

    return NULL;
}