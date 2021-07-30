#ifndef MQTT64M_IF_H
#define MQTT64M_IF_H

/*
MQTT TOPIC:

LW0_TEMP = xxx.x(C)
LW0_VRMS = _xx.x(mg)
LW0_VAVG = _xx.x(mg)
LW0_PEAK = __xxx(HZ)

*/

const uint8_t Protocal_Name[MQTT_PROTOCAL_LENGTH] = {'M', 'Q', 'I', 's', 'd', 'p'};
const uint8_t Client_Name[MQTT_CLIENT_ID_LENGTH] = {'T', 'H', 'U', '_', 'R', 'X', '6', '4', 'M', '_', 'I', 'O', 'T'};


uint16_t MSG_IDENTIFIER = 0x0010;


typedef enum {
	CONNECT_CMD = 0x10,
	SUBSCRIBE_CMD = 0x82,
	PUBLISH_AT_MOST_ONCE = 0x30,
	PUBLISH_AT_LEAST_ONCE = 0x32,
	PUBLISH_EXACTLY_ONCE = 0x34,
	DISCONNECT_CMD = 0xE0
}MQTT_CONNECT_TYPE;

typedef enum {
	AT_MOST_ONCE,
	AT_LEAST_ONCE,
	EXACTLY_ONCE,
}MQTT_QOS_TYPE;


#endif