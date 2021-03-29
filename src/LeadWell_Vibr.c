
#include "mainHeader.h"

extern volatile uint8_t vibrRtnRAWData[VIBR_SENS_RETURN_LENGTH];
extern volatile int16_t vibrateData[VIBR_DATA_LENGTH];
extern volatile int32_t frequencyData[FREQ_DATA_LENGTH];
extern volatile int16_t EDSData[EDS_DATA_LENGTH];
extern volatile uint16_t TempData[TEMP_DATA_LENGTH];

volatile int8_t HLvibrateData[HL_VIBR_DATA_LENGTH];
volatile int8_t HLfrequencyData[HL_FREQ_DATA_LENGTH];
volatile int8_t HLEDSData[HL_EDS_DATA_LENGTH];
volatile int8_t HLTempData[HL_TEMP_DATA_LENGTH];


void main(void){	/*MAIN FUNCTION START POINT*/
/* WIRESHARK
dhcp && !ip.addr == 10.0.0.178 && dhcp.id == 0x12345678
tcp && tcp.flags.syn && ip.dst == 10.0.0.178 && ip.src == 10.0.0.0/24
*/
	printf("Bootup Process\n");
	printf("==========INIT==========\n");
	
	Rx64MInitPorts();
	
#if (ENABLE_UART == MODE_DISABLE) && ((ENABLE_VIBR == MODE_ENABLE) || (ENABLE_EDS == MODE_ENABLE))
	#error "-- INIT Configuration Check Failed!";
	#error "-- You need to enable UART before enable vibration oe EDS";
#endif


#if ENABLE_UART == MODE_ENABLE
	printf("-- INIT: [Enabled] UART\n");
	UARTInit();
#else
	printf("-- INIT: [Disabled] UART\n");
#endif


#if ENABLE_VIBR == MODE_ENABLE
	printf("-- INIT: [Enabled] UART/CH_VIBR\n");
#else
	printf("-- INIT: [Disabled] UART/CH_VIBR\n");
#endif


#if ENABLE_EDS == MODE_ENABLE
	printf("-- INIT: [Enabled] UART/CH_EDS\n");
#else
	printf("-- INIT: [Disabled] UART/CH_EDS\n");
#endif


#if ENABLE_DSP == MODE_ENABLE
	printf("-- INIT: [Enabled] DSP\n");
	DspInit();
#else
	printf("-- INIT: [Disabled] DSP\n");
#endif
	

#if ENABLE_ADC == MODE_ENABLE
	printf("-- INIT: [Enabled] ADC\n");
	ADCInit();
#else
	printf("-- INIT: [Disabled] ADC\n");
#endif


#if ENABLE_ETHERNET == MODE_ENABLE
	printf("-- INIT: [Enabled] Ethernet\n");
	printf("-- INIT: Ethernet Link Process\n");
	
	EthernetInit(ENABLE_DHCP);
	while(!ETHERNET_RDY[0]){
		printf("        Waiting for Plug in signal...\n");
		R_ETHER_LinkProcess(0);
		R_BSP_SoftwareDelay (500, BSP_DELAY_MILLISECS);
	}
#else
	printf("-- INIT: [Disabled] Ethernet\n");
#endif


#if ENABLE_ETHERNET == MODE_ENABLE
	TCP_Connect(encodeIPv4(TAR_SERVER_IP), TAR_SERVER_PT, TCP_CONNID_TD);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
	//TCP_Connect(encodeIPv4(TAR_SERVER_IP), TAR_SERVER_PT, TCP_CONNID_FD);
	//R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
#endif
	
	printf("========================\n");
	printf("BOOTED\n");
	
/*MAIN WHILE LOOP START POINT*/	
    while(1){

#if ENABLE_VIBR == MODE_ENABLE
		vibrSensorSend();
		R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
		vibrSensorProcess(0);
		R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
		conv_int16_int8(vibrateData, HLvibrateData, VIBR_DATA_LENGTH);
#endif

#if ENABLE_DSP == MODE_ENABLE
		printf(">> Calculate FFT Data\n");
		R_DSP_REAL_FFT_Operation(vibrateData, frequencyData);
		R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
#endif


#if ENABLE_EDS == MODE_ENABLE
		edsSensorSend();
		R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
		edsSensorProcess(0);
		R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
#endif



#if ENABLE_ADC == MODE_ENABLE
	TempData[0] = 0xDDDD;
	printf(">> ADC0 = %d(0x%X)\n",TempData[1],TempData[1]);
	printf(">> ADC1 = %d(0x%X)\n",TempData[2],TempData[2]);
	conv_int16_int8(TempData, HLTempData, TEMP_DATA_LENGTH);
	R_BSP_SoftwareDelay (200, BSP_DELAY_MILLISECS);
#endif



#if ENABLE_ETHERNET == MODE_ENABLE

	#if ENABLE_VIBR == MODE_ENABLE
		printf(">> TCP Sending: HLvibrateData\n");
		HLvibrateData[0] = 0xAA;
		TCP_SendingData(TCP_CONNID_TD, HLvibrateData, HL_VIBR_DATA_LENGTH);
		R_BSP_SoftwareDelay (1, BSP_DELAY_SECS);
		printf(">> TCP Sending Completed\n\n");
	#endif

	#if ENABLE_DSP == MODE_ENABLE
		printf(">> TCP Sending: frequencyData\n");
		frequencyData[0] = 0xBBBBBBBB;
		TCP_SendingData(TCP_CONNID_TD, frequencyData, HL_FREQ_DATA_LENGTH);
		R_BSP_SoftwareDelay (1, BSP_DELAY_SECS);
		printf(">> TCP Sending Completed\n\n");
	#endif
	
	#if ENABLE_EDS == MODE_ENABLE
		printf(">> TCP Sending: EDSData\n");
		TCP_SendingData(TCP_CONNID_TD, EDSData, (EDS_DATA_LENGTH) * 2);
		R_BSP_SoftwareDelay (1, BSP_DELAY_SECS);
		printf(">> TCP Sending Completed\n\n");
	#endif
	
	#if ENABLE_ADC == MODE_ENABLE
		printf(">> TCP Sending: HLTempData\n");
		TCP_SendingData(TCP_CONNID_TD, HLTempData, (TEMP_DATA_LENGTH) * 2);
		R_BSP_SoftwareDelay (1, BSP_DELAY_SECS);
		printf(">> TCP Sending Completed\n\n");
	#endif
#endif


#if FLUSH_BUF_AFTER_SENDING == MODE_ENABLE
		printf(">> Flushing vibrRtnRAWData\n");
		flushBuffer(vibrRtnRAWData, VIBR_SENS_RETURN_LENGTH);
		printf(">> Flushing vibrateData\n");
		flushBuffer(vibrateData, VIBR_DATA_LENGTH);
		printf(">> Flushing frequencyData\n");
		flushBuffer(frequencyData, VIBR_DATA_LENGTH / 2);
		printf(">> Flushing EDSRtnRAWData\n");
		flushBuffer(EDSRtnRAWData, EDS_SENS_RETURN_LENGTH);
		printf(">> Flushing EDSData\n");
		flushBuffer(EDSData, EDS_DATA_LENGTH);
		printf(">> Flushing TempData\n");
		flushBuffer(TempData, TEMP_DATA_LENGTH);
		
		printf(">> Flushing HLEDSData\n");
		flushBuffer(HLEDSData, TEMP_DATA_LENGTH * 2);
#endif
		printf(">> One Loop\n");
		R_BSP_SoftwareDelay(LOOPIN_DELAY_TIME, BSP_DELAY_SECS);
    }/*MAIN WHILE LOOP END POINT*/
}/*MAIN FUNCTION END POINT*/

void Rx64MInitPorts(void){
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC); // Use BSP function to unlock the MPC register.
/*
	LED2_PDR = MODE_IO_OUTPUT;
	LED3_PDR = MODE_IO_OUTPUT;
	LED4_PDR = MODE_IO_OUTPUT;
	LED5_PDR = MODE_IO_OUTPUT;
*/	
	/*Setting for P40 as AN000*/
	MPC.P40PFS.BYTE = 0x80;
        PORT4.PDR.BIT.B0 = 0;
        PORT4.PMR.BIT.B0 = 1;
	/*Setting for P41 as AN001*/
	MPC.P41PFS.BYTE = 0x80;
        PORT4.PDR.BIT.B1 = 0;
        PORT4.PMR.BIT.B1 = 1;
	
	/*Sensor*/
	/*For SCI-UART_ch1*/
        /* Set RXD1 pin */  /*P15*/
        MPC.P15PFS.BYTE = 0x0A;
        PORT1.PMR.BIT.B5 = 0x01;    // Set P15 mode to peripheral operation
        /* Set TXD1 pin */  /*P16*/
        PORT1.PODR.BIT.B6 = 0x00;
        MPC.P16PFS.BYTE = 0x0A;
        PORT1.PMR.BIT.B6 = 0x01;  // Set P16 mode to peripheral operation
        PORT1.PDR.BIT.B6 = 0x01;
	
	/*For SCI-UART_ch2*/
        /* Set RXD2 pin */  /*P12*/
        MPC.P12PFS.BYTE = 0x0A;
        PORT1.PMR.BIT.B2 = 0x01;    // Set P12 mode to peripheral operation
        /* Set TXD2 pin */  /*P13*/
        PORT1.PODR.BIT.B3 = 0x00;
        MPC.P13PFS.BYTE = 0x0A;
        PORT1.PMR.BIT.B3 = 0x01;  // Set P13 mode to peripheral operation
        PORT1.PDR.BIT.B3 = 0x01;
	
	
	/*For SCI-UART_ch2*/
        /* Set RXD2 pin */  /*P52*/
        //MPC.P52PFS.BYTE = 0x0A;
        //PORT5.PMR.BIT.B2 = 0x01;    // Set P52 mode to peripheral operation
        /* Set TXD2 pin */  /*P50*/
        //PORT5.PODR.BIT.B0 = 0x00;
        //MPC.P50PFS.BYTE = 0x0A;
        //PORT5.PMR.BIT.B0 = 0x01;  // Set P50 mode to peripheral operation
        //PORT5.PDR.BIT.B0 = 0x01;
	
	/*For SCI-UART_ch1*/
        /* Set RXD1 pin */  /*P15*/
        //MPC.P15PFS.BYTE = 0x0A;
        //PORT1.PMR.BIT.B5 = 0x01;    // Set P15 mode to peripheral operation
        /* Set TXD1 pin */  /*P16*/
        //PORT1.PODR.BIT.B6 = 0x00;
        //MPC.P16PFS.BYTE = 0x0A;
        //PORT1.PMR.BIT.B6 = 0x01;  // Set P16 mode to peripheral operation
        //PORT1.PDR.BIT.B6 = 0x01;

        return;
}

void conv_int16_int8(int16_t * inputBuffer, int8_t * outputBuffer, uint16_t length){
	printf(">>> Convert int16 to int8\n");
		
	for(int idx = 0; idx < length; idx++){
		*(outputBuffer + 2*idx) = (*(inputBuffer + idx) >> 8 ) & 0xFF;	//High Byte
		*(outputBuffer + 2*idx+1) = (*(inputBuffer + idx) & 0xFF);	//Low Byte
	}
}

void conv_int32_int8(int32_t * inputBuffer, int8_t * outputBuffer, uint16_t length){
	printf(">>> Convert int16 to int8\n");
		
	for(int idx = 0; idx < length; idx++){
		*(outputBuffer + 4*idx) = (*(inputBuffer + idx) >> 24 ) & 0xFF;		//First Byte
		*(outputBuffer + 4*idx+1) = (*(inputBuffer + idx) >> 16 ) & 0xFF;	//Second Byte
		*(outputBuffer + 4*idx+2) = (*(inputBuffer + idx) >> 8 ) & 0xFF;	//Third Byte
		*(outputBuffer + 4*idx+3) = (*(inputBuffer + idx) >> 0 ) & 0xFF;	//Forth Byte
	}
}