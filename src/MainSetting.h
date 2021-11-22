#ifndef MAIN_SETTING_H
#define MAIN_SETTING_H

//MODE_ENABLE | MODE_DISABLE

//Internet Settings
#define		ENABLE_ETHERNET			(MODE_ENABLE)
#define		ENABLE_DHCP			(MODE_ENABLE)
#define		ENABLE_MQTT			(MODE_ENABLE)
#define		ENABLE_TCP			(MODE_ENABLE)
#define		ENABLE_IRQ			(MODE_ENABLE)
#define		USING_DEGBUG_TCP_SERVER		(MODE_DISABLE)
#define		USING_DEGBUG_MQTT_SERVER	(MODE_DISABLE)
#define		USING_MQTT_PRIVATE_IP		(MODE_ENABLE)

#define		DEGUB_SERVER_IP			10,0,0,178
#define		REAL_SERVER_IP			172,31,37,246		
#define		MQTT_PRIVATE_IP			210,66,194,20		//LeadWell MQTT Private IP
#define		MQTT_PUBLIC_IP			210,241,243,252		//LeadWell MQTT Public IP

//Sensor Settings
#define		ENABLE_UART			(MODE_ENABLE)
#define		ENABLE_VIBR			(MODE_ENABLE)
#define		ENABLE_DSP			(MODE_ENABLE)
#define		ENABLE_EDS			(MODE_ENABLE)
#define		ENABLE_ADC			(MODE_ENABLE)

//Other Settings
#define		FLUSH_BUF_AFTER_SENDING		(MODE_ENABLE)
#define		PRINT_DEBUGGING_MESSAGE		(MODE_ENABLE)


/*Techinically, these settings below should not be modified*/
#define		TCP_CONNID		1
#define		MQTT_CONNID		2


#if USING_DEGBUG_TCP_SERVER == MODE_ENABLE
	#define		TCP_SERVER_IP		DEGUB_SERVER_IP
#else
	#define		TCP_SERVER_IP		REAL_SERVER_IP
#endif
#define		TCP_SERVER_PT		8120

#if USING_DEGBUG_MQTT_SERVER == MODE_ENABLE
	#define		MQTT_SERVER_IP		DEGUB_SERVER_IP
#else
	#if USING_MQTT_PRIVATE_IP == MODE_ENABLE
		#define		MQTT_SERVER_IP		MQTT_PRIVATE_IP
	#else
		#define		MQTT_SERVER_IP		MQTT_PUBLIC_IP
	#endif
#endif

#define		MQTT_SERVER_PT		1883

#define		LOOPIN_DELAY_TIME	(500)			//Mili Second
#define		TCP_SEND_DELAY_TIME	(150)			//Mili Second
#define		SENSOR_DELAY_TIME	(100)			//Mili Second


#endif