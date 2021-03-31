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

#define DATA_RDY_IND_LENGTH			(5)

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
#define DATA_RDY_IND_TEMP		(4)

extern volatile uint8_t vibrRtnRAWData[VIBR_SENS_RETURN_LENGTH];
extern volatile uint8_t EDSRtnRAWData[EDS_SENS_RETURN_LENGTH];

extern volatile int16_t vibrateData[VIBR_DATA_LENGTH];
extern volatile int32_t frequencyData[VIBR_DATA_LENGTH / 2];
extern volatile int16_t EDSData[EDS_DATA_LENGTH];

extern volatile uint16_t TempData[TEMP_DATA_LENGTH];

extern bool ETHERNET_RDY[2];
extern uint8_t DATA_RDY[DATA_RDY_IND_LENGTH];

#endif