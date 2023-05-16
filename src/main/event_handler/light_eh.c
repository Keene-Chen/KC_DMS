#include "kc_dms.h"

void light_handler(void* client, message_data_t* msg)
{
    (void)client;
    // MQTT_LOG_I("%s:%d %s()...\r\ntopic: %s\r\nmessage:%s\r\n", __FILE__, __LINE__, __FUNCTION__,
    //            msg->topic_name, (char*)msg->message->payload);
    yyjson_doc* doc =
        yyjson_read((char*)msg->message->payload, strlen((char*)msg->message->payload), 0);

    if (doc) {
        yyjson_val* light = yyjson_obj_get(doc->root, "light");
        if (yyjson_get_real(light) > 300) {
            delay_s_dev(LED_DEV, 1);
        }
    }
    yyjson_doc_free(doc);
}