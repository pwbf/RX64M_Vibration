
#include "mainHeader.h"

extern volatile uint8_t vibrRtnRAWData[VIBR_SENS_RETURN_LENGTH];
extern volatile int16_t vibrateData[VIBR_DATA_LENGTH];
extern volatile int32_t frequencyData[FREQ_DATA_LENGTH];
extern volatile uint16_t EDSData[EDS_DATA_LENGTH];
extern volatile uint16_t EDS2Data[EDS_DATA_LENGTH];
extern volatile uint16_t TempData[TEMP_DATA_LENGTH];

extern volatile int8_t HLvibrateData[HL_VIBR_DATA_LENGTH];
extern volatile int8_t HLTempData[HL_TEMP_DATA_LENGTH];

extern bool ProgramStatusIndicator;

extern const uint8_t pfx_avg[MQTT_VAVG_PREFIX_LENGTH];
extern const uint8_t pfx_pk[MQTT_PEAK_PREFIX_LENGTH];
extern const uint8_t pfx_rms[MQTT_VRMS_PREFIX_LENGTH];
extern const uint8_t pfx_temp[MQTT_TEMP_PREFIX_LENGTH];
extern const uint8_t iu_avg[MQTT_STRUCT_UNIT_LENGTH];
extern const uint8_t iu_pk[MQTT_STRUCT_UNIT_LENGTH];
extern const uint8_t iu_pkval[MQTT_STRUCT_UNIT_LENGTH];
extern const uint8_t iu_rms[MQTT_STRUCT_UNIT_LENGTH];
extern const uint8_t iu_temp[MQTT_STRUCT_UNIT_LENGTH];
extern const uint8_t Topic_LW0_TEMP[MQTT_TOPIC_LENGTH];
extern const uint8_t Topic_LW0_VRMS[MQTT_TOPIC_LENGTH];
extern const uint8_t Topic_LW0_VAVG[MQTT_TOPIC_LENGTH];
extern const uint8_t Topic_LW0_PEAK[MQTT_TOPIC_LENGTH];
extern const uint8_t Topic_LW0_SEDS[MQTT_TOPIC_LENGTH];
extern const uint8_t Topic_LW0_2EDS[MQTT_TOPIC_LENGTH];
extern const uint8_t Topic_THU_TEST[MQTT_TOPIC_LENGTH];

extern volatile VIBR_MISC vm;
extern volatile MQTT_VIBR mv;
extern volatile MQTT_PAYLOAD mp;

void main(void){	/*MAIN FUNCTION START POINT*/
/* WIRESHARK
dhcp && !ip.addr == 10.0.0.178 && dhcp.id == 0x12345678
tcp && tcp.flags.syn && ip.dst == 10.0.0.178 && ip.src == 10.0.0.0/24
*/
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("Bootup Process\n");
		printf("==========INIT==========\n");
	#endif
	
	flushBuffer(&vm, sizeof(vm));
	flushBuffer(&mv, sizeof(mv));
	flushBuffer(&mp, sizeof(mp));
	
	Rx64MInitPorts();
	LEDinit();
	
#if (ENABLE_UART == MODE_DISABLE) && ((ENABLE_VIBR == MODE_ENABLE) || (ENABLE_EDS == MODE_ENABLE))
	#error "-- INIT Configuration Check Failed!";
	#error "-- You need to enable UART before enable vibration or EDS";
#endif

#if (ENABLE_ETHERNET == MODE_DISABLE) && ((ENABLE_MQTT == MODE_ENABLE) || (ENABLE_TCP == MODE_ENABLE))
	#error "-- INIT Configuration Check Failed!";
	#error "-- You need to enable Ethernet before enable MQTT or TCP";
#endif


#if ENABLE_IRQ == MODE_ENABLE
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Enabled] TCP Terminate Button\n");
	#endif
	R_r_irq_rx_Create();
	R_r_irq_rx_IRQ10_Start();
#else
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Disabled] TCP Terminate Button\n");
	#endif
#endif


#if ENABLE_UART == MODE_ENABLE
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Enabled] UART\n");
	#endif
	UARTInit();
#else
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Disabled] UART\n");
	#endif
#endif


#if ENABLE_VIBR == MODE_ENABLE
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Enabled] UART/CH_VIBR\n");
	#endif
#else
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Disabled] UART/CH_VIBR\n");
	#endif
#endif


#if ENABLE_EDS == MODE_ENABLE
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Enabled] UART/CH_EDS\n");
	#endif
#else
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Disabled] UART/CH_EDS\n");
	#endif
#endif


#if ENABLE_DSP == MODE_ENABLE
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Enabled] DSP\n");
	#endif
	DspInit();
#else
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Disabled] DSP\n");
	#endif
#endif
	

#if ENABLE_ADC == MODE_ENABLE
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Enabled] ADC\n");
	#endif
	ADCInit();
#else
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Disabled] ADC\n");
	#endif
#endif


#if ENABLE_ETHERNET == MODE_ENABLE
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Enabled] Ethernet\n");
		printf("-- INIT: Ethernet Link Process\n");
	#endif
	
	
	EthernetInit(ENABLE_DHCP);
	while(!ETHERNET_RDY[0]){
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf("        Waiting for Plug in signal...\n");
		#endif
		R_ETHER_LinkProcess(0);
		R_BSP_SoftwareDelay (500, BSP_DELAY_MILLISECS);
	}
#else
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Disabled] Ethernet\n");
	#endif
#endif


#if ENABLE_TCP == MODE_ENABLE
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Enabled] TCP Client\n");
	#endif
#else
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Disabled] TCP Client\n");
	#endif
#endif

#if ENABLE_MQTT == MODE_ENABLE
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Enabled] MQTT Client\n");
	#endif
#else
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: [Disabled] MQTT Client\n");
	#endif
#endif


#if ENABLE_ETHERNET == MODE_ENABLE
	#if ENABLE_TCP == MODE_ENABLE
		int8_t tcp_connection_status = TCP_Connect(encodeIPv4(TCP_SERVER_IP), TCP_SERVER_PT, TCP_CONNID);
		while(tcp_connection_status != 0 && tcp_connection_status != -57){
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf("        Waiting for TCP Server Respond...\n");
			#endif
			R_BSP_SoftwareDelay (500, BSP_DELAY_MILLISECS);
			tcp_connection_status = TCP_Connect(encodeIPv4(TCP_SERVER_IP), TCP_SERVER_PT, TCP_CONNID);
		}
		LED_ETH_ST	= LED_ON;
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf("        TCP server Connection Established!!\n");
		#endif
	#endif
	
	#if ENABLE_MQTT == MODE_ENABLE
		int8_t mqtt_connection_status = TCP_Connect(encodeIPv4(MQTT_SERVER_IP), MQTT_SERVER_PT, MQTT_CONNID);
		while(mqtt_connection_status != 0 && mqtt_connection_status != -57){
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf("        Waiting for MQTT Server Respond...\n");
			#endif
			R_BSP_SoftwareDelay (500, BSP_DELAY_MILLISECS);
			mqtt_connection_status = TCP_Connect(encodeIPv4(MQTT_SERVER_IP), MQTT_SERVER_PT, MQTT_CONNID);
		}
		LED_ETH_ST	= LED_ON;
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf("        MQTT server TCP Connection Established!!\n");
		#endif
		
		MQTT_connect(MQTT_CONNID);
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf("        MQTT server Connection Established!!\n");
		#endif
		
		MQTT_Packet_Init();
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf("        MQTT Payload initialized!!\n");
		#endif
	#endif
#endif
	
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("========================\n");
		printf("BOOTED\n");
	#endif
	
/*MAIN WHILE LOOP START POINT*/	
	ProgramStatusIndicator = 1;
    while(ProgramStatusIndicator){
	LED_ETH_ST	= LED_ON;
	LED_IP_ST	= LED_ON;
	
	if(ProgramStatusIndicator == 0)
		break;
	
#if ENABLE_ETHERNET == MODE_ENABLE
	/**Check Ethernet is connected!**/
	while(!ETHERNET_RDY[0]){
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf("        Waiting for Plug in signal...\n");
		#endif
		R_ETHER_LinkProcess(0);
		R_BSP_SoftwareDelay (500, BSP_DELAY_MILLISECS);
	}
#endif

#if ENABLE_VIBR == MODE_ENABLE
		LED_VB_TX	= LED_ON;
		vibrSensorSend();
		LED_VB_TX	= LED_OFF;
		R_BSP_SoftwareDelay (SENSOR_DELAY_TIME, BSP_DELAY_MILLISECS);
		
		LED_VB_RX	= LED_ON;
		vibrSensorProcess(0);
		LED_VB_RX	= LED_OFF;
		R_BSP_SoftwareDelay (SENSOR_DELAY_TIME, BSP_DELAY_MILLISECS);
		conv_int16_int8(vibrateData, HLvibrateData, VIBR_DATA_LENGTH);
		arrayAverage(vibrateData, VIBR_DATA_LENGTH);
		arrayRMS(vibrateData, VIBR_DATA_LENGTH);
		arrayMaximumTD(vibrateData, VIBR_DATA_LENGTH);
#endif

#if ENABLE_DSP == MODE_ENABLE
		if(DATA_RDY[DATA_RDY_IND_VIBR]){
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">> Calculate FFT Data\n");
			#endif
			R_DSP_REAL_FFT_Operation(vibrateData, frequencyData);
			DATA_RDY[DATA_RDY_IND_FREQ] = STATE_TRUE;
			R_BSP_SoftwareDelay (SENSOR_DELAY_TIME, BSP_DELAY_MILLISECS);
			arrayMaximumFD(frequencyData, VIBR_DATA_LENGTH / 2);
			
		}
#endif


#if ENABLE_EDS == MODE_ENABLE
		LED_EDS_TX	= LED_ON;
		edsSensorSend();
		edsSensorSend2();
		LED_EDS_TX	= LED_OFF;
		R_BSP_SoftwareDelay (SENSOR_DELAY_TIME, BSP_DELAY_MILLISECS);
		
		LED_EDS_RX	= LED_ON;
		edsSensorProcess(0);
		eds2SensorProcess(0);
		LED_EDS_RX	= LED_OFF;
		R_BSP_SoftwareDelay (SENSOR_DELAY_TIME, BSP_DELAY_MILLISECS);
		
		vm.eds = EDSData[1];
#endif



#if ENABLE_ADC == MODE_ENABLE
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf(">> ADC0 = %d(0x%X)\n",TempData[1],TempData[1]);
		printf(">> ADC1 = %d(0x%X)\n",TempData[2],TempData[2]);
	#endif
	conv_int16_int8(TempData, HLTempData, TEMP_DATA_LENGTH);
	R_BSP_SoftwareDelay (SENSOR_DELAY_TIME, BSP_DELAY_MILLISECS);
#endif

//#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
	printf("\n-----------------------\n");
	printf("Pre-Sending Data Check:\n");
	printf("[%d] Vibration Data\n", DATA_RDY[DATA_RDY_IND_VIBR]);
	printf("[%d] Frequency Data\n", DATA_RDY[DATA_RDY_IND_FREQ]);
	printf("[%d] EDS Data\n", DATA_RDY[DATA_RDY_IND_EDS]);
	printf("[%d] EDS2 Data\n", DATA_RDY[DATA_RDY_IND_EDS2]);
	printf("[%d] Temperature Data\n", DATA_RDY[DATA_RDY_IND_TEMP]);
	printf("Peak Freq:%d\n", vm.pk);
	printf("Peak Value:%f\n", vm.pkval);
	printf("RMS Value:%f\n", vm.rms);
	printf("AVG Value:%f\n", vm.avg);
	printf("Temp Value:%f\n", vm.temp);
	printf("EDS Value:%d vm:0x%04X EDS:0x%04X\n", vm.eds,vm.eds,EDSData[1]);
	printf("EDS2 Value:%d vm:0x%04X EDS:0x%04X\n", vm.eds2,vm.eds2,EDS2Data[1]);
	printf("-----------------------\n\n");
//#endif

#if ENABLE_TCP == MODE_ENABLE

	#if ENABLE_VIBR == MODE_ENABLE
		if(DATA_RDY[DATA_RDY_IND_VIBR]){
			LED_VB_TX	= LED_ON;
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">> TCP Sending: HLvibrateData\n");
			#endif
			HLvibrateData[0] = 0xAA;
			#if ENABLE_TCP == MODE_ENABLE
				TCP_SendingData(TCP_CONNID, HLvibrateData, HL_VIBR_DATA_LENGTH);
			#endif
			R_BSP_SoftwareDelay (TCP_SEND_DELAY_TIME, BSP_DELAY_MILLISECS);
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">> TCP Sending Completed\n\n");
			#endif
		}
		else{
			LED_VB_TX	= LED_OFF;
		}
	#endif

	#if ENABLE_DSP == MODE_ENABLE
		if(DATA_RDY[DATA_RDY_IND_FREQ]){
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">> TCP Sending: frequencyData\n");
			#endif
			frequencyData[0] = 0xBBBBBBBB;
			#if ENABLE_TCP == MODE_ENABLE
				TCP_SendingData(TCP_CONNID, frequencyData, HL_FREQ_DATA_LENGTH);
			#endif
			R_BSP_SoftwareDelay (TCP_SEND_DELAY_TIME, BSP_DELAY_MILLISECS);
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">> TCP Sending Completed\n\n");
			#endif
		}
	#endif
	
	#if ENABLE_EDS == MODE_ENABLE
		if(DATA_RDY[DATA_RDY_IND_EDS]){
			LED_EDS_TX	= LED_ON;
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">> TCP Sending: EDSData\n");
			#endif
			EDSData[0] = 0xCCCC;
			#if ENABLE_TCP == MODE_ENABLE
				TCP_SendingData(TCP_CONNID, EDSData, (EDS_DATA_LENGTH) * 2);
			#endif
			R_BSP_SoftwareDelay (TCP_SEND_DELAY_TIME, BSP_DELAY_MILLISECS);
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">> TCP Sending Completed\n\n");
			#endif
		}
		else{
			LED_EDS_TX	= LED_OFF;
		}
	#endif
	
	#if ENABLE_ADC == MODE_ENABLE
		if(DATA_RDY[DATA_RDY_IND_TEMP]){
			LED_TEMP_1	= LED_ON;
			LED_TEMP_2	= LED_ON;
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">> TCP Sending: HLTempData\n");
			#endif
			HLTempData[0] = 0xDD;
			#if ENABLE_TCP == MODE_ENABLE
				TCP_SendingData(TCP_CONNID, HLTempData, (TEMP_DATA_LENGTH) * 2);
			#endif
			R_BSP_SoftwareDelay (TCP_SEND_DELAY_TIME, BSP_DELAY_MILLISECS);
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">> TCP Sending Completed\n\n");
			#endif
		}
		else{
			LED_TEMP_1	= LED_OFF;
			LED_TEMP_2	= LED_OFF;
		}
	#endif
	
	if(
		!(
			DATA_RDY[DATA_RDY_IND_VIBR] && 
			DATA_RDY[DATA_RDY_IND_FREQ] && 
			DATA_RDY[DATA_RDY_IND_EDS] && 
			DATA_RDY[DATA_RDY_IND_EDS2] && 
			DATA_RDY[DATA_RDY_IND_TEMP]
		)
	){
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> TCP Sending: Error Logging\n");
		#endif
		DATA_RDY[0] = 0xEE;
		#if ENABLE_TCP == MODE_ENABLE
			TCP_SendingData(TCP_CONNID, DATA_RDY, DATA_RDY_IND_LENGTH);
		#endif
		R_BSP_SoftwareDelay (TCP_SEND_DELAY_TIME, BSP_DELAY_MILLISECS);
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> TCP Sending Completed\n\n");
		#endif
	}
	
#endif


#if ENABLE_MQTT == MODE_ENABLE
	sprintf(mv.avg, "%5.1f", vm.avg);
	sprintf(mv.pk, "%5d", vm.pk);
	sprintf(mv.pkval, " %4.1f", vm.pkval);
	sprintf(mv.eds, "%5d", vm.eds);
	sprintf(mv.eds2, "%5d", vm.eds2);
	sprintf(mv.rms, "%5.1f", vm.rms);
	sprintf(mv.temp, "%5.1f", vm.temp);
	
	if(DATA_RDY[DATA_RDY_IND_VIBR]){
		MQTT_publish(MQTT_CONNID, &Topic_LW0_VAVG, &(mv.avg), MQTT_STRUCT_VALUE_LENGTH);
	}
		
	if(DATA_RDY[DATA_RDY_IND_VIBR]){
		MQTT_publish(MQTT_CONNID, &Topic_LW0_PEAK, &(mv.pkval), MQTT_STRUCT_VALUE_LENGTH);
	}
	
	if(DATA_RDY[DATA_RDY_IND_VIBR]){
		MQTT_publish(MQTT_CONNID, &Topic_LW0_VRMS, &(mv.rms), MQTT_STRUCT_VALUE_LENGTH);
	}
	
	if(!DATA_RDY[DATA_RDY_IND_EDS]){
		MQTT_publish(MQTT_CONNID, &Topic_LW0_SEDS, &(mv.eds), MQTT_STRUCT_VALUE_LENGTH);
	}
	
	if(!DATA_RDY[DATA_RDY_IND_EDS2]){
		MQTT_publish(MQTT_CONNID, &Topic_LW0_2EDS, &(mv.eds2), MQTT_STRUCT_VALUE_LENGTH);
	}
	
	if(DATA_RDY[DATA_RDY_IND_TEMP]){
		MQTT_publish(MQTT_CONNID, &Topic_LW0_TEMP, &(mv.temp), MQTT_STRUCT_VALUE_LENGTH);
	}
#endif


#if FLUSH_BUF_AFTER_SENDING == MODE_ENABLE
	LED_StepUP();
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing vibrRtnRAWData\n");
		#endif
		flushBuffer(vibrRtnRAWData, VIBR_SENS_RETURN_LENGTH);
		
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing vibrateData\n");
		#endif
		flushBuffer(vibrateData, VIBR_DATA_LENGTH);
		
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing HLvibrateData\n");
		#endif
		flushBuffer(HLvibrateData, VIBR_DATA_LENGTH);
		
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing frequencyData\n");
		#endif
		
		flushBuffer(frequencyData, VIBR_DATA_LENGTH / 2);
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing EDSRtnRAWData\n");
		#endif
		flushBuffer(EDSRtnRAWData, EDS_SENS_RETURN_LENGTH);
		
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing EDSData\n");
		#endif
		flushBuffer(EDSData, EDS_DATA_LENGTH);
		
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing EDS2RtnRAWData\n");
		#endif
		flushBuffer(EDS2RtnRAWData, EDS_SENS_RETURN_LENGTH);
		
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing EDS2Data\n");
		#endif
		flushBuffer(EDS2Data, EDS_DATA_LENGTH);
		
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing TempData\n");
		#endif
		flushBuffer(TempData, TEMP_DATA_LENGTH);
		
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing HLTempData\n");
		#endif
		flushBuffer(HLTempData, TEMP_DATA_LENGTH);
		
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> Flushing Data Indicator\n");
		#endif
		flushBuffer(DATA_RDY, DATA_RDY_IND_LENGTH);
		
	LED_StepDN();
#endif
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> One Loop\n");
		#endif
		
	LED_TEMP_1	= LED_OFF;
	LED_TEMP_2	= LED_OFF;
	LED_VB_TX	= LED_OFF;
	LED_VB_RX	= LED_OFF;
	LED_EDS_TX	= LED_OFF;
	LED_EDS_RX	= LED_OFF;
	
		R_BSP_SoftwareDelay(LOOPIN_DELAY_TIME, BSP_DELAY_MILLISECS);
		
    }/*MAIN WHILE LOOP END POINT*/
    
#if ENABLE_MQTT == MODE_ENABLE
	printf("Terminating MQTT Connection...\n");
	MQTT_terminate(MQTT_CONNID);
	TCP_Terminate(MQTT_CONNID);
	printf("OK\n");
#endif

#if ENABLE_TCP == MODE_ENABLE
	printf("Terminating TCP Connection...\n");
	TCP_Terminate(TCP_CONNID);
	printf("OK\n");
#endif
    printf("Ended!\n");
    
    /*Intercept the ended program here*/
LED_ETH_ST	= LED_ON;
LED_IP_ST	= LED_OFF;
LED_TEMP_1	= LED_ON;
LED_TEMP_2	= LED_OFF;
LED_VB_TX	= LED_ON;
LED_VB_RX	= LED_OFF;
LED_EDS_TX	= LED_ON;
LED_EDS_RX	= LED_OFF;
    while(1); /*DO NOT ADD ANYTHING BELOW THIS LINE*/
}/*MAIN FUNCTION END POINT*/

void MQTT_Packet_Init(void){
	write2array(&(mp.avg), &pfx_avg, MQTT_VAVG_PREFIX_LENGTH, 0);
	write2array(&(mp.pk), &pfx_pk, MQTT_PEAK_PREFIX_LENGTH, 0);
	write2array(&(mp.rms), &pfx_rms, MQTT_VRMS_PREFIX_LENGTH, 0);
	write2array(&(mp.temp), &pfx_temp, MQTT_TEMP_PREFIX_LENGTH, 0);
	
	write2array(&(mp.avg), &iu_avg, MQTT_STRUCT_UNIT_LENGTH, MQTT_VAVG_PREFIX_LENGTH + MQTT_STRUCT_VALUE_LENGTH);
	write2array(&(mp.rms), &iu_rms, MQTT_STRUCT_UNIT_LENGTH, MQTT_VRMS_PREFIX_LENGTH + MQTT_STRUCT_VALUE_LENGTH);
	write2array(&(mp.temp), &iu_temp, MQTT_STRUCT_UNIT_LENGTH, MQTT_TEMP_PREFIX_LENGTH + MQTT_STRUCT_VALUE_LENGTH);
	
}

void Rx64MInitPorts(void){
        R_BSP_RegisterProtectDisable(BSP_REG_PROTECT_MPC); // Use BSP function to unlock the MPC register.
	
	/*Setting for P40 as AN007*/
	MPC.P47PFS.BYTE = 0x80;
        PORT4.PDR.BIT.B7 = 0;
        PORT4.PMR.BIT.B7 = 1;
	/*Setting for P41 as AN006*/
	MPC.P46PFS.BYTE = 0x80;
        PORT4.PDR.BIT.B6 = 0;
        PORT4.PMR.BIT.B6 = 1;
	
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
	
        return;
}

uint16_t swapHLbyte(uint16_t n){
    uint8_t hibyte = (n & 0xff00) >> 8;
    uint8_t lobyte = (n & 0xff);
    
    return lobyte << 8 | hibyte;
}

void write2array(uint8_t *target, uint8_t *source, uint8_t length, uint8_t offset){
    for(uint8_t index = 0; index < length; index++){
    	*(target + offset + index) = *(source + index);
	//printf("Target Address: 0x%X, Old: 0x%X, New: 0x%X\n",target + offset + index, *(target + offset + index), *(source + index));
    }
}

void conv_int16_int8(int16_t * inputBuffer, int8_t * outputBuffer, uint16_t length){
	for(int idx = 0; idx < length; idx++){
		*(outputBuffer + 2*idx) = (*(inputBuffer + idx) >> 8 ) & 0xFF;	//High Byte
		*(outputBuffer + 2*idx+1) = (*(inputBuffer + idx) & 0xFF);	//Low Byte
	}
}

void conv_int32_int8(int32_t * inputBuffer, int8_t * outputBuffer, uint16_t length){		
	for(int idx = 0; idx < length; idx++){
		*(outputBuffer + 4*idx) = (*(inputBuffer + idx) >> 24 ) & 0xFF;		//First Byte
		*(outputBuffer + 4*idx+1) = (*(inputBuffer + idx) >> 16 ) & 0xFF;	//Second Byte
		*(outputBuffer + 4*idx+2) = (*(inputBuffer + idx) >> 8 ) & 0xFF;	//Third Byte
		*(outputBuffer + 4*idx+3) = (*(inputBuffer + idx) >> 0 ) & 0xFF;	//Forth Byte
	}
}