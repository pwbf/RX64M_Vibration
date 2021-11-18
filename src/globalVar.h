#ifndef GLOBAL_VAR_H
#define GLOBAL_VAR_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "platform.h"


//Data Length [HEADER][DATA0][DATA1]...
// 1+ for not rewrite first data
#define	VIBR_SENS_RETURN_LENGTH			(2055)			//Returned Data Length
#define	EDS_SENS_RETURN_LENGTH			(9)			//Returned Data Length

#define	VIBR_DATA_LENGTH			(1024)			//Vibration Data Length
#define	FREQ_DATA_LENGTH			VIBR_DATA_LENGTH / 2	//Vibration Data Length
#define	EDS_DATA_LENGTH				(1 + 2)			//EDS Data Length
#define TEMP_DATA_LENGTH			(1 + 2)			//Temperature Data Length

#define	HL_VIBR_DATA_LENGTH			VIBR_DATA_LENGTH * 2	//Vibration Data Length (HL Byte)
#define	HL_FREQ_DATA_LENGTH			FREQ_DATA_LENGTH * 4	//Frequency Data Length (HL Byte)
#define	HL_EDS_DATA_LENGTH			EDS_DATA_LENGTH * 2	//EDS Data Length (HL Byte)
#define HL_TEMP_DATA_LENGTH			TEMP_DATA_LENGTH * 2	//Temperature Data Length (HL Byte)

#define DATA_RDY_IND_LENGTH			(6)

#define MQTT_VAVG_PREFIX_LENGTH			(9)
#define MQTT_PEAK_PREFIX_LENGTH			(6)
#define MQTT_VRMS_PREFIX_LENGTH			(5)
#define MQTT_SEDS_PREFIX_LENGTH			(5)
#define MQTT_TEMP_PREFIX_LENGTH			(13)
#define MQTT_STRUCT_VALUE_LENGTH		(5)
#define MQTT_STRUCT_UNIT_LENGTH			(4)
#define MQTT_TOPIC_LENGTH			8

#define MQTT_VAVG_PAYLOAD_LENGTH		MQTT_VAVG_PREFIX_LENGTH + MQTT_STRUCT_VALUE_LENGTH + MQTT_STRUCT_UNIT_LENGTH	
#define MQTT_VPEAK_PAYLOAD_LENGTH		MQTT_PEAK_PREFIX_LENGTH + MQTT_STRUCT_VALUE_LENGTH + MQTT_STRUCT_UNIT_LENGTH + MQTT_STRUCT_VALUE_LENGTH + MQTT_STRUCT_UNIT_LENGTH
#define MQTT_SEDS_PAYLOAD_LENGTH		MQTT_SEDS_PREFIX_LENGTH + MQTT_STRUCT_VALUE_LENGTH + MQTT_STRUCT_UNIT_LENGTH
#define MQTT_VRMS_PAYLOAD_LENGTH		MQTT_VRMS_PREFIX_LENGTH + MQTT_STRUCT_VALUE_LENGTH + MQTT_STRUCT_UNIT_LENGTH
#define	MQTT_TEMP_PAYLOAD_LENGTH		MQTT_TEMP_PREFIX_LENGTH + MQTT_STRUCT_VALUE_LENGTH + MQTT_STRUCT_UNIT_LENGTH

//Globe Statement Definition
#define		MODE_ENABLE		(1)
#define		MODE_DISABLE		(0)
#define		MODE_IO_OUTPUT		(1)
#define		MODE_IO_INPUT		(0)
#define		LED_OFF			(1)
#define		LED_ON			(0)
#define		STATE_TRUE		(1)
#define		STATE_FALSE		(0)

#define DATA_RDY_IND_VIBR		(1)
#define DATA_RDY_IND_FREQ		(2)
#define DATA_RDY_IND_EDS		(3)
#define DATA_RDY_IND_EDS2		(4)
#define DATA_RDY_IND_TEMP		(5)


#define 	RECEIVE_SIZE		700

extern volatile uint8_t vibrRtnRAWData[VIBR_SENS_RETURN_LENGTH];
extern volatile uint8_t EDSRtnRAWData[EDS_SENS_RETURN_LENGTH];
extern volatile uint8_t EDS2RtnRAWData[EDS_SENS_RETURN_LENGTH];

extern volatile int16_t vibrateData[VIBR_DATA_LENGTH];
extern volatile int32_t frequencyData[VIBR_DATA_LENGTH / 2];
extern volatile uint16_t EDSData[EDS_DATA_LENGTH];
extern volatile uint16_t EDS2Data[EDS_DATA_LENGTH];

extern volatile uint16_t TempData[TEMP_DATA_LENGTH];

extern bool ETHERNET_RDY[2];
extern uint8_t DATA_RDY[DATA_RDY_IND_LENGTH];


typedef struct vibrmisc{
	float avg;		//Average(mg)
	uint16_t pk;		//Peak Frequency(Hz)
	uint16_t eds;		//EDS
	uint16_t eds2;		//EDS
	float pkval;		//Peak Value(mg)
	float rms;		//RMS(mg)
	float temp;		//Temperature(*C)
}VIBR_MISC;

typedef struct mqttvibr{
	uint8_t	 avg[MQTT_STRUCT_VALUE_LENGTH];		//Average(mg)	
	uint8_t  pk[MQTT_STRUCT_VALUE_LENGTH];		//Peak Frequency(Hz)
	uint8_t  pkval[MQTT_STRUCT_VALUE_LENGTH];	//Peak Value(mg)
	uint8_t  eds[MQTT_STRUCT_VALUE_LENGTH];		//EDS	
	uint8_t  eds2[MQTT_STRUCT_VALUE_LENGTH];	//EDS2	
	uint8_t  rms[MQTT_STRUCT_VALUE_LENGTH];		//RMS(mg)	
	uint8_t  temp[MQTT_STRUCT_VALUE_LENGTH];	//Temperature(*C)
}MQTT_VIBR;

typedef struct mqttpayload{
	uint8_t	 avg[MQTT_VAVG_PAYLOAD_LENGTH];		//Average(mg)	
	uint8_t  pk[MQTT_VPEAK_PAYLOAD_LENGTH];		//Peak Frequency(Hz)Value(mg)	
	uint8_t  eds[MQTT_SEDS_PAYLOAD_LENGTH];		//EDS	
	uint8_t  eds2[MQTT_SEDS_PAYLOAD_LENGTH];	//EDS2	
	uint8_t  rms[MQTT_VRMS_PAYLOAD_LENGTH];		//RMS(mg)	
	uint8_t  temp[MQTT_TEMP_PAYLOAD_LENGTH];	//Temperature(*C)
}MQTT_PAYLOAD;

#endif