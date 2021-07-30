#include "platform.h"
#include "globalVar.h"

#include "MainSetting.h"

#include "MQTT.h"
#include "MQTT_if.h"


MQTT_CONNECT_TYPE mct;
MQTT_QOS_TYPE mqos;


#pragma section _MQTT_SEC
	MQTT_CONN_PACKET mcp;
	MQTT_PACKET_SIZE smcp = sizeof(mcp) - 1;
	MQTT_PUB_PACKET mpp;
	MQTT_PACKET_SIZE smpp = sizeof(mpp) - 1;
	MQTT_TERM_PACKET mtp;
	MQTT_PACKET_SIZE smtp = sizeof(mtp) - 0;
	MQTT_SUB_PACKET msp;
	MQTT_PACKET_SIZE smsp = sizeof(msp) - 1;
#pragma section
	
void MQTT_connect(uint8_t tcp_id){
	
	mcp.mqtt_header = CONNECT_CMD;
	mcp.msg_length = smcp - 2;
	mcp.prt_length = swapHLbyte(0x06);
	write2array(&(mcp.prt_name), &Protocal_Name, MQTT_PROTOCAL_LENGTH, 0);
	mcp.mqtt_version = 0x03;
	mcp.connect_flags = 0x02;
	mcp.keep_alive = swapHLbyte(0x78);
	mcp.client_id_length = swapHLbyte(0x0d);
	write2array(&(mcp.client_id), &Client_Name, MQTT_CLIENT_ID_LENGTH, 0);
	
	printf(">> MQTT Connect\n");
	
	TCP_SendingData(tcp_id, &mcp, smcp);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
}

void MQTT_subscribe(uint8_t tcp_id){
	
	msp.mqtt_header = SUBSCRIBE_CMD;
	msp.msg_length = smsp - 2;
	msp.msd_id = swapHLbyte(MSG_IDENTIFIER++);
	msp.topic_length = swapHLbyte(MQTT_TOPIC_LENGTH);
//	write2array(&(msp -> topic_name), &Topic_THU_TEST, MQTT_TOPIC_LENGTH, 0);
	msp.req_qos = AT_MOST_ONCE;
	
	TCP_SendingData(tcp_id, &msp, smsp);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
}

void MQTT_publish(uint8_t tcp_id, uint8_t *topic, uint8_t *payload, uint8_t payloadLength){
	mpp.mqtt_header = PUBLISH_AT_MOST_ONCE;
	mpp.msg_length = smpp - 2;
	mpp.topic_length = swapHLbyte(MQTT_TOPIC_LENGTH);
	write2array(&(mpp.topic_name), topic, MQTT_TOPIC_LENGTH, 0);
	write2array(&(mpp.message), payload, payloadLength, 0);
	
	TCP_SendingData(tcp_id, &mpp, smpp);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
		flushBuffer(&mpp, smpp);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
}

void MQTT_terminate(uint8_t tcp_id){
	
	mtp.mqtt_header = DISCONNECT_CMD;
	mtp.msg_length = 0;
	
	TCP_SendingData(tcp_id, &mtp, smtp);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
}

uint16_t swapHLbyte(uint16_t n){
    uint8_t hibyte = (n & 0xff00) >> 8;
    uint8_t lobyte = (n & 0xff);
    
    return lobyte << 8 | hibyte;
}

void write2array(uint8_t *target, uint8_t *source, uint8_t length, uint8_t offset){
    for(uint8_t index = 0; index < length; index++)
    	*(target + offset + index) = *(source + index);
}