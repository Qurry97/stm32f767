#ifndef  _MQTT_H_
#define  _MQTT_H_ 

#include "qx_rtt.h"

#define CLIENT_ID  "gaf3ST8qGba.stm32|securemode=2\\,signmethod=hmacsha256\\,timestamp=1652853721294|"           //CLIENT_ID逗号前不加反斜杠，发送AT+MQTTUSERCFG会报错
#define USER_NAME  "stm32&gaf3ST8qGba"
#define PASSWORD   "5239f61f3cfb281080568ce097b38f170d2911d6abdf591f8ce6cb61c22b6e76"
#define HOST_URL   "iot-06z00iwb245n13s.mqtt.iothub.aliyuncs.com"
#define PORT       "1883"
#define WIFI_ID    "OnePlus9R"
#define WIFI_PW    "13582467"
#define PUB_TOPIC  "/sys/gaf3ST8qGba/stm32/thing/event/property/post"
#define SUB_TOPIC  "/sys/gaf3ST8qGba/stm32/thing/event/property/post_reply"

#define VERSION    "\\\"version\\\":\\\"1.0\\\""
#define METHOD     "\\\"method\\\":\\\"thing.event.property.post\\\""


extern uint8_t mqtt_init(char *wifi_id, char * wifi_pw);
extern void mqtt_connect_aliyun(char * host_url, char *port, char * client_id, char * user_name, char * device_pw);
extern void mqtt_close_connect();
extern void mqtt_sub_topic(char * Topic);
extern void mqtt_unsub_topic(char * Topic);
extern void mqtt_publish_topic(char * Topic ,char * Data);

#endif
