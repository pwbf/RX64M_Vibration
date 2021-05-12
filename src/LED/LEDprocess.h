#ifndef LED_CONFIG_H
#define LED_CONFIG_H

#include "platform.h"
#include "globalVar.h"
#include "MainSetting.h"

#define		LED_COUNT		(8)

#define		LED_TEMP_1		PORTE.PODR.BIT.B2
#define		LED_TEMP_2		PORTE.PODR.BIT.B1
#define		LED_VB_TX		PORTE.PODR.BIT.B0
#define		LED_VB_RX		PORT6.PODR.BIT.B4
#define		LED_EDS_TX		PORT6.PODR.BIT.B3
#define		LED_EDS_RX		PORT6.PODR.BIT.B2
#define		LED_ETH_ST		PORT6.PODR.BIT.B1
#define		LED_IP_ST		PORTD.PODR.BIT.B7

#define		PDR_TEMP_1		PORTE.PDR.BIT.B2
#define		PDR_TEMP_2		PORTE.PDR.BIT.B1
#define		PDR_VB_TX		PORTE.PDR.BIT.B0
#define		PDR_VB_RX		PORT6.PDR.BIT.B4
#define		PDR_EDS_TX		PORT6.PDR.BIT.B3
#define		PDR_EDS_RX		PORT6.PDR.BIT.B2
#define		PDR_ETH_ST		PORT6.PDR.BIT.B1
#define		PDR_IP_ST		PORTD.PDR.BIT.B7


void LEDinit(void);
void LED_AllON(void);
void LED_AllOFF(void);
void LED_StepUP(void);
void LED_StepDN(void);
void LED_LidODD(void);
void LED_LidEVN(void);

#endif