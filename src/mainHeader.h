#ifndef MAINHEADER_H
#define MAINHEADER_H

/*
//	You should NOT modify definetion below!!
//	All customizable settings are in MainSetting.h
//	--by PWBF 2021.3.22
*/

#include "typedefine.h"
#include "iodefine.h"
#include "platform.h"
#include "globalVar.h"


/*For LED*/
#include "LEDprocess.h"

/*For SCI*/
#include "r_sci_rx_if.h"        // The SCI module API interface file.
#include "r_sci_rx_config.h"    // User configurable options for the SCI module

/*For ADC*/
#include "r_s12ad_rx_config.h"
#include "r_s12ad_rx_if.h"

/*For Sensor*/
#include "Sensor.h"

/*For DSP Library*/
#include "dsp_header.h"

/*For Ethernet Process*/
#include "EthernetProcess.h"

/*For MQTT*/
#include "MQTT.h"

/*For IRQ*/
#include "r_irq_rx.h"

/*For RMS AVG PEAK Calculation*/
#include "MiscClac.h"

/*For Mode or Function Switching*/
#include "MainSetting.h"

void conv_int16_int8(int16_t * inputBuffer, int8_t * outputBuffer, uint16_t length);
void conv_int32_int8(int32_t * inputBuffer, int8_t * outputBuffer, uint16_t length);
void Rx64MInitPorts(void);
void MQTT_Packet_Init(void);
uint16_t swapHLbyte(uint16_t n);
void write2array(uint8_t *target, uint8_t *source, uint8_t length, uint8_t offset);

#endif