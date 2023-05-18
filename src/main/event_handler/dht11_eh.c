#include "kc_dms.h"

void dht11_handler(void* client, message_data_t* msg)
{
    (void)client;

    // MQTT_LOG_I("%s:%d %s()...\r\ntopic: %s\r\nmessage:%s\r\n", __FILE__, __LINE__, __FUNCTION__,
    //            msg->topic_name, (char*)msg->message->payload);
    yyjson_doc* doc = yyjson_read((char*)msg->message->payload, strlen(msg->message->payload), 0);
    if (doc) {
        yyjson_val* temp = yyjson_obj_get(doc->root, "temp");
        yyjson_val* humi = yyjson_obj_get(doc->root, "humi");
        if (yyjson_get_real(temp) > TEMP_ALERT || yyjson_get_real(humi) > HUMI_ALERT) {
            open_dev(BLUE_LED);
        }
        else {
            close_dev(BLUE_LED);
        }
    }
    yyjson_doc_free(doc);
}