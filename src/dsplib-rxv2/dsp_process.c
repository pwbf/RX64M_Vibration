#include "dsp_header.h"

extern volatile int32_t frequencyData[512];

void DspInit(void){
	r_dsp_status_t status = R_DSP_STATUS_OK;
	idx_buf_s12ad %= BUF_NUM_BLOCKS_FEED;
	gs_request_s12ad = NO_REQUEST;
	
	memset((void *)frequencyData, 0, sizeof(frequencyData));  /* Clears Output Buffer to zero */
	R_DSP_REAL_FFT_Init();
}