#ifndef MAIN_SETTING_H
#define MAIN_SETTING_H

#define		TCP_CONNID_TD		1
#define		TCP_CONNID_FD		2
#define		SRC_CLIENT_IP		0,0,0,0			//This define seems unused
#define		TAR_SERVER_IP		172,31,37,246
//#define		TAR_SERVER_IP		10,0,0,178
#define		TAR_SERVER_PT		8120

#define		LOOPIN_DELAY_TIME	(10)			//Second

//MODE_ENABLE | MODE_DISABLE
//Functions
#define		ENABLE_ETHERNET			(MODE_ENABLE)
#define		ENABLE_DHCP			(MODE_ENABLE)
#define		ENABLE_UART			(MODE_ENABLE)
#define		ENABLE_VIBR			(MODE_ENABLE)
#define		ENABLE_DSP			(MODE_ENABLE)
#define		ENABLE_EDS			(MODE_ENABLE)
#define		ENABLE_ADC			(MODE_ENABLE)

//Mode
#define		FLUSH_BUF_AFTER_SENDING		(MODE_ENABLE)

#endif