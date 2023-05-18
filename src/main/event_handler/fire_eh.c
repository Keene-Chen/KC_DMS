#include "kc_dms.h"

void fire_handler(void* client, message_data_t* msg)
{
    (void)client;
    // MQTT_LOG_I("%s:%d %s()...\r\ntopic: %s\r\nmessage:%s\r\n", __FILE__, __LINE__, __FUNCTION__,
    //            msg->topic_name, (char*)msg->message->payload);

    yyjson_doc* doc = yyjson_read((char*)msg->message->payload, strlen(msg->message->payload), 0);
    if (doc) {
        yyjson_val* fire = yyjson_obj_get(doc->root, "raw");
        if (yyjson_get_int(fire) < FIRE_ALERT) {
            open_dev(RED_LED);
            open_dev(BEEP_DEV);
        }
        else {
            close_dev(RED_LED);
            close_dev(BEEP_DEV);
        }
    }
    yyjson_doc_free(doc);
}