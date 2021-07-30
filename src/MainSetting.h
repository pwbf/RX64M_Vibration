#ifndef MAIN_SETTING_H
#define MAIN_SETTING_H

#define		TCP_CONNID		1
#define		MQTT_CONNID		2
#define		TCP_SERVER_IP		172,31,37,246
//#define		TCP_SERVER_IP		10,0,0,178
#define		TCP_SERVER_PT		8120
//#define		MQTT_SERVER_IP		10,0,0,178
#define		MQTT_SERVER_IP		210,241,243,252
#define		MQTT_SERVER_PT		1883

#define		LOOPIN_DELAY_TIME	(500)			//Mili Second
#define		TCP_SEND_DELAY_TIME	(150)			//Mili Second
#define		SENSOR_DELAY_TIME	(100)			//Mili Second

//MODE_ENABLE | MODE_DISABLE
//Functions
#define		ENABLE_ETHERNET			(MODE_ENABLE)
#define		ENABLE_DHCP			(MODE_ENABLE)
#define		ENABLE_MQTT			(MODE_ENABLE)
#define		ENABLE_TCP			(MODE_ENABLE)
#define		ENABLE_IRQ			(MODE_ENABLE)

#define		ENABLE_UART			(MODE_ENABLE)
#define		ENABLE_VIBR			(MODE_ENABLE)
#define		ENABLE_DSP			(MODE_ENABLE)
#define		ENABLE_EDS			(MODE_ENABLE)
#define		ENABLE_ADC			(MODE_ENABLE)

//Mode
#define		FLUSH_BUF_AFTER_SENDING		(MODE_ENABLE)
#define		PRINT_DEBUGGING_MESSAGE		(MODE_ENABLE)

#endif