#include "Sensor.h"

extern volatile uint8_t vibrRtnRAWData[VIBR_SENS_RETURN_LENGTH];
extern volatile int16_t vibrateData[VIBR_DATA_LENGTH];

extern volatile uint8_t EDSRtnRAWData[EDS_SENS_RETURN_LENGTH];
extern volatile int16_t EDSData[EDS_DATA_LENGTH];

uint8_t *p_EDSRtnRAWData = EDSRtnRAWData;
int16_t *p_EDSData = EDSData;

uint8_t *p_vibrRtnRAWData = vibrRtnRAWData;
int16_t *p_vibrateData = vibrateData;


uint8_t vibrSensorSend(void){
	printf(">> Send Command Vibr\n");
	uint8_t cmd[HOST_SEND_LENGTH]={
		SENS_ST, 
		VIBR_SENS_ID, 
		VIBR_SENS_CMD, 
		SENS_TB0, 
		SENS_TB1, 
		SENS_TB2, 
		SENS_TB3, 
		VIBR_SENS_CHK, 
		SENS_END
	};
	return R_SCI_Send(VIBR_UART_HANDLE, cmd, HOST_SEND_LENGTH);
}

uint8_t edsSensorSend(void){
	printf(">> Send Command EDS\n");
	uint8_t cmd[HOST_SEND_LENGTH]={
		SENS_ST, 
		EDS_SENS_ID, 
		EDS_SENS_CMD, 
		SENS_TB0, 
		SENS_TB1, 
		SENS_TB2, 
		SENS_TB3, 
		EDS_SENS_CHK, 
		SENS_END
	};
	return R_SCI_Send(EDS_UART_HANDLE, cmd, HOST_SEND_LENGTH);
}

void flushBuffer(uint8_t * aryBuffer, uint16_t length){
	for(int idx = 0; idx < length; idx++){
		// printf("Original Data: 0x%02X", *(aryBuffer + idx));
		*(aryBuffer + idx) = 0x00;
	}
}

void vibrSensorProcess(uint8_t status){
	uint16_t index = 0;
	int8_t rtnRecieve;
	uint8_t rtnbyte;
	
	printf(">> Recieve Vibration Data--->\n");
	while(status == SCI_SUCCESS){
		rtnRecieve = R_SCI_Receive(VIBR_UART_HANDLE,&rtnbyte,1);
		if(rtnRecieve == SCI_SUCCESS){
			*(p_vibrRtnRAWData + index++) = rtnbyte;
			if((*(p_vibrRtnRAWData + (VIBR_SENS_RETURN_LENGTH - 1)) != 0)){
				printf(">> Vibration buffer p+2054 = %d\n", *(p_vibrRtnRAWData + (VIBR_SENS_RETURN_LENGTH - 1)));
				mergeHLbyte(p_vibrRtnRAWData, p_vibrateData, VIBR_DATA_LENGTH);
				break;
			}
			// printf(".");
		}
		R_BSP_SoftwareDelay (1, BSP_DELAY_MILLISECS);
	}	
}

void edsSensorProcess(uint8_t status){
	uint16_t index = 0;
	int8_t rtnRecieve;
	uint8_t rtnbyte;
	
	printf(">> Recieve EDS Data--->\n");
	while(status == SCI_SUCCESS){
		rtnRecieve = R_SCI_Receive(EDS_UART_HANDLE,&rtnbyte,1);
		if(rtnRecieve == SCI_SUCCESS){
			*(p_EDSRtnRAWData + index++) = rtnbyte;
			printf(">> EDS = %d(0x%02X)\n", rtnbyte, rtnbyte);
			if((*(p_EDSRtnRAWData + (EDS_SENS_RETURN_LENGTH - 1)) != 0)){
				printf(">> EDS buffer p+8 = %d\n", *(p_EDSRtnRAWData + (EDS_SENS_RETURN_LENGTH - 1)));
				mergeHLbyte(p_EDSRtnRAWData, p_EDSData, EDS_DATA_LENGTH);
				break;
			}
			 // printf(".");
		}
		R_BSP_SoftwareDelay (10, BSP_DELAY_MILLISECS);
	}	
}

void mergeHLbyte(uint8_t * inputBuffer, int16_t * outputBuffer, uint16_t length){
	/* 5L-6H -> 2051L-2052H */
	/*   0         >1024    */
	printf(">> Merging High Low Data\n");
	for(int idx = 0; idx < length; idx++){
		*(outputBuffer + idx) = *(inputBuffer + (2*idx) + 5) | *(inputBuffer + (2*idx + 1) + 5) *256;
		// printf("(2*idx)+5 = %d | (2*idx+1)+6 = %d\n", idx, *(outputBuffer + idx) );
		// printf("%d\n", *(outputBuffer + idx) );
	}
}

void UARTInit(void){
        sci_cfg_t   my_sci_config;
        sci_err_t   my_sci_err_ch1;
        sci_err_t   my_sci_err_ch2;

    /**********************************************************************************/
        /* Set up the configuration data structure for asynchronous (UART) operation. */
        my_sci_config.async.baud_rate    = 9600;
        my_sci_config.async.clk_src      = SCI_CLK_INT;
        my_sci_config.async.data_size    = SCI_DATA_8BIT;
        my_sci_config.async.parity_en    = SCI_PARITY_OFF;
        my_sci_config.async.parity_type  = SCI_EVEN_PARITY;
        my_sci_config.async.stop_bits    = SCI_STOPBITS_1;
        my_sci_config.async.int_priority = 15;    // 1=lowest, 15=highest

        /* OPEN ASYNC CHANNEL
        *  Provide address of the config structure,
        *  the callback function to be assigned,
        *  and the location for the handle to be stored.*/     
		
	my_sci_err_ch1 = R_SCI_Open(SCI_CH1, SCI_MODE_ASYNC, &my_sci_config, my_sci_callback_ch1, &g_my_sci_handle_ch1);
	my_sci_err_ch2 = R_SCI_Open(SCI_CH2, SCI_MODE_ASYNC, &my_sci_config, my_sci_callback_ch2, &g_my_sci_handle_ch2);

        /* If there were an error this would demonstrate error detection of API calls. */
		
	if (SCI_SUCCESS != my_sci_err_ch1)
        {
			printf("SCI_Open_Ch1_Error!!\n");
        }
	if (SCI_SUCCESS != my_sci_err_ch2)
        {
			printf("SCI_Open_Ch2_Error!!\n");
        }
}

void my_sci_callback_ch1(void *pArgs){
	sci_cb_args_t   *args;

    args = (sci_cb_args_t *)pArgs;

    if (args->event == SCI_EVT_RX_CHAR)
    {
        /* From RXI interrupt; received character data is in args->byte */
        nop();
    }
    else if (args->event == SCI_EVT_RXBUF_OVFL)
    {
        /* From RXI interrupt; rx queue is full; 'lost' data is in args->byte
           You will need to increase buffer size or reduce baud rate */
        nop();
    }
    else if (args->event == SCI_EVT_OVFL_ERR)
    {
        /* From receiver overflow error interrupt; error data is in args->byte
           Error condition is cleared in calling interrupt routine */
        nop();
    }
    else if (args->event == SCI_EVT_FRAMING_ERR)
    {
        /* From receiver framing error interrupt; error data is in args->byte
           Error condition is cleared in calling interrupt routine */
        nop();
    }
    else if (args->event == SCI_EVT_PARITY_ERR)
    {
        /* From receiver parity error interrupt; error data is in args->byte
           Error condition is cleared in calling interrupt routine */
        nop();
    }
}
void my_sci_callback_ch2(void *pArgs){
	sci_cb_args_t   *args;

    args = (sci_cb_args_t *)pArgs;

    if (args->event == SCI_EVT_RX_CHAR)
    {
        /* From RXI interrupt; received character data is in args->byte */
        nop();
    }
    else if (args->event == SCI_EVT_RXBUF_OVFL)
    {
        /* From RXI interrupt; rx queue is full; 'lost' data is in args->byte
           You will need to increase buffer size or reduce baud rate */
        nop();
    }
    else if (args->event == SCI_EVT_OVFL_ERR)
    {
        /* From receiver overflow error interrupt; error data is in args->byte
           Error condition is cleared in calling interrupt routine */
        nop();
    }
    else if (args->event == SCI_EVT_FRAMING_ERR)
    {
        /* From receiver framing error interrupt; error data is in args->byte
           Error condition is cleared in calling interrupt routine */
        nop();
    }
    else if (args->event == SCI_EVT_PARITY_ERR)
    {
        /* From receiver parity error interrupt; error data is in args->byte
           Error condition is cleared in calling interrupt routine */
        nop();
    }
}

void init_mtu0(void){
    SYSTEM.PRCR.WORD = 0xA502;
    MSTP(MTU0) = 0;             // Release from module stop state.
    SYSTEM.PRCR.WORD = 0xA500;

    MTU.TSTRA.BYTE = 0x00;      // Stop MTU counters

    // Timer Control Reg (TCR)
    // CCLR[2:0] = 001: TCNT cleared by TGRA compare match/input capture
    // CKEG[1:0] = 00 : Count at rising edge
    // TPSC[2:0] = 011: Time Prescaler Select; Internal clock: counts on PCLKA/64
    MTU0.TCR.BYTE = 0x23;

    MTU0.TIORH.BYTE = 0x00;     // IO control A/B output prohibited
    MTU0.TIORL.BYTE = 0x00;     // IO control C/D output prohibited

    MTU0.TMDR1.BYTE = 0x00;     // Normal mode

    MTU0.TCNT = 0;              // Clear timer counter
    MTU0.TGRA = 60000;          // Set the compare match count value

    MTU0.TIER.BIT.TTGE = 1;     // Trigger ADC on TRGA compare match (TRGA0N sync trigger)

    /* Enable the TGIA0 (TGRA input capture/compare match) interrupt which will
     * cause the mtu0_tgia0_isr() ISR below to be called each time a TGRA compare
     * match occurs. Note that this is not required to periodically start the ADC
     * conversion (that occurs because MTU0.TIER.BIT.TTGE is set above whenever a
     * compare match occurs). */
    IR(MTU0,TGIA0) = 0;         // Clear the interrupt status flag
    IPR(MTU0,TGIA0) = 9;        // Set the interrupt source priority
    IEN(MTU0,TGIA0) = 1;        // Enable the interrupt request
    MTU0.TIER.BIT.TGIEA = 1;    // Enable TGIA interrupts

    MTU.TSTRA.BYTE = 0x01;      // Start counting on MTU0
}

void ADCInit(void){
        adc_err_t       adc_err;
        adc_err_t       adc_err0;
        adc_err_t       adc_err1;
        adc_cfg_t       config;     // Open() configuration
        adc_ch_cfg_t    ch_cfg;     // Control() channel configuration

        /* Clear all members of the adc_cfg_t structure */
        memset(&config, 0, sizeof(config));
        config.resolution =  ADC_RESOLUTION_12_BIT;     // 12 bit resolution
        config.alignment = ADC_ALIGN_RIGHT;             // conversions are right-aligned
        config.add_cnt = ADC_ADD_OFF;                   // no addition on conversions
        config.clearing = ADC_CLEAR_AFTER_READ_OFF;     // data remains in register after reading
        config.trigger = ADC_TRIG_SYNC_TRG0AN;          // trigger from mtu
        config.trigger_groupb = ADC_TRIG_NONE;          // group mode not used
        config.priority = 15;                            // interrupt priority 3 (1=low, 15=high)
        config.priority_groupb = 0;                     // group mode not used

        adc_err = R_ADC_Open(USING_ADC_UNIT, ADC_MODE_SS_MULTI_CH, &config, ADC_READ);
        while (ADC_SUCCESS != adc_err){
            /* Error - loop here for debug */
        }

        /* Enable channel */                 
	ch_cfg.chan_mask = MASK_ADC_CH0|MASK_ADC_CH1; 
	//ch_cfg.chan_mask = ADC_MASK_CH1|ADC_MASK_CH0;
	//ch_cfg.chan_mask = ADC_MASK_CH8|ADC_MASK_CH9|ADC_MASK_CH10|ADC_MASK_CH11;
        ch_cfg.priority_groupa = ADC_GRPA_PRIORITY_OFF;      // group mode not used
        ch_cfg.chan_mask_groupb = ADC_MASK_GROUPB_OFF;       // group mode not used
        ch_cfg.add_mask = ADC_MASK_ADD_OFF;                  // no channels using addition
        ch_cfg.diag_method = ADC_DIAG_OFF;                   // self-diagnosis off
        ch_cfg.anex_enable = false;                          // no external amplifier
        ch_cfg.sample_hold_mask = ADC_MASK_SAMPLE_HOLD_OFF;  // Bypass chnl-dedicated S&H circuits
        ch_cfg.sample_hold_states = ADC_SST_SH_CNT_DEFAULT;  // default sample & hold states

        adc_err = R_ADC_Control(USING_ADC_UNIT, ADC_CMD_ENABLE_CHANS, &ch_cfg);
        while (ADC_SUCCESS != adc_err){
        /* Error - loop here for debug */
			printf("adc_error");
        }
	ADCStart();
		
}

void ADCStart(void){
        adc_err_t       adc_err;

        /* Initialize trigger source (MTU0) */
        init_mtu0();

        /* Enable triggers in ADC */   /*Enables to start A/D conversion with TRIG, */
        adc_err = R_ADC_Control(USING_ADC_UNIT, ADC_CMD_ENABLE_TRIG, FIT_NO_PTR);
        while (ADC_SUCCESS != adc_err){
        /* Error - loop here for debug */
        }

}

void ADC_READ(void *pArgs){
    // adc_err_t adc_err;
    adc_err_t adc_scan_err;
    adc_err_t adc_err0;
    adc_err_t adc_err1;
    adc_cb_args_t *args = (adc_cb_args_t *)pArgs;

    if ((0 == args->unit) && (ADC_EVT_SCAN_COMPLETE == args->event)){
        /* Read conversion value and store in global variable */
		adc_err0 = R_ADC_Read(USING_ADC_UNIT, TEMP_ADC_CH0, &TempData[1]);
		adc_err1 = R_ADC_Read(USING_ADC_UNIT, TEMP_ADC_CH1, &TempData[2]);
        while (ADC_SUCCESS != adc_err0){
            /* Error - loop here for debug */
        }
    }
}