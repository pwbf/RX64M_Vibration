#ifndef SENSOR_H
#define SENSOR_H

#include "platform.h"
#include "globalVar.h"

/*For SCI*/
#include "r_sci_rx_if.h"        // The SCI module API interface file.
#include "r_sci_rx_config.h"    // User configurable options for the SCI module

/*For ADC*/
#include "r_s12ad_rx_config.h"
#include "r_s12ad_rx_if.h"

#include "MainSetting.h"

#define HOST_SEND_LENGTH		(9)

#define SENS_ST				(37)		//Start code
#define VIBR_SENS_ID			(255)		//Vibration Sensor Address ID
#define EDS_SENS_ID			(1)		//EDS Sensor Address ID
#define SENS_TB0			(0)		//Sensor TX bytes 0
#define SENS_TB1			(0)		//Sensor TX bytes 1
#define SENS_TB2			(0)		//Sensor TX bytes 2
#define SENS_TB3			(0)		//Sensor TX bytes 3
#define VIBR_SENS_CMD			(64)		//Vibration Command
#define VIBR_SENS_CHK			(100)		//Vibration Check Sum
#define EDS_SENS_CMD			(23)		//EDS Command
#define EDS_SENS_CHK			(61)		//EDS Check Sum
#define SENS_END			(35)		//End code

#define EDS_RTN_B0			(0)		//EDS return byte 0
#define EDS_RTN_B1			(1)		//EDS return byte 1
#define EDS_RTN_B2			(2)		//EDS return byte 2				
#define EDS_RTN_B3			(3)		//EDS return byte 3

#define USING_ADC_UNIT			0			//ADC Channel(AN0xx or AN1xx)

#define VIBR_UART_HANDLE		g_my_sci_handle_ch1	//Vibration UART handle function
#define EDS_UART_HANDLE			g_my_sci_handle_ch2	//EDS UART handle function
#define TEMP_ADC_CH0			ADC_REG_CH0		//Temperature channel 0 ADC reg (AN00x or AN10x) ADC_REG_CHx
#define TEMP_ADC_CH1			ADC_REG_CH1		//Temperature channel 1 ADC reg (AN00x or AN10x) ADC_REG_CHx
#define MASK_ADC_CH0			ADC_MASK_CH0		//Temperature channel 0 ADC reg (AN00x or AN10x) ADC_REG_CHx
#define MASK_ADC_CH1			ADC_MASK_CH1		//Temperature channel 1 ADC reg (AN00x or AN10x) ADC_REG_CHx

#define UART_TIMEOUT			(1000)


static sci_hdl_t   g_my_sci_handle_ch1;
static sci_hdl_t   g_my_sci_handle_ch2;

uint8_t vibrSensorSend(void);
uint8_t edsSensorSend(void);
void vibrSensorProcess(uint8_t status);
void edsSensorProcess(uint8_t status);
void flushBuffer(uint8_t * aryBuffer, uint16_t length);
void mergeHLbyte(uint8_t * inputBuffer, int16_t * outputBuffer, uint16_t length, uint16_t inShift, uint16_t outShift);
void UARTInit(void);
void my_sci_callback_ch1(void *pArgs);
void my_sci_callback_ch2(void *pArgs);

void init_mtu0(void);
void ADCInit(void);
void ADCStart(void);
void ADC_READ(void *pArgs);

extern uint8_t DATA_RDY[DATA_RDY_IND_LENGTH];

#endif