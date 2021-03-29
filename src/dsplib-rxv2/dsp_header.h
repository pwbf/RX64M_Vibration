#ifndef DSP_HEADER_H
#define DSP_HEADER_H

#include "typedefine.h"
#include "iodefine.h"
#include "platform.h"
#include "globalVar.h"

#include "r_dsp_types.h"
#include "r_dsp_transform.h"
#define FFT_POINTS (1024)
#define MY_MIN_DSP_LIB_VERSION    (0x05000000u)
#define BUF_NUM_SAMPLES     (FFT_POINTS)    /* Specifies sample count of a block of Input Buffer */
#define BUF_NUM_BLOCKS      (8)             /* Specifies block count of Input Buffer */
#define BUF_NUM_INFO        (2)             /* Specifies element count of Buffer Information array, must be 2 */

#define CURRENT             (0)             /* Identifier for 1st. element, must be 0 */
#define NEXT                (1)             /* Identifier for 2nd. element, must be 1 */
#define BUF_NUM_BLOCKS_FEED (BUF_NUM_BLOCKS/2)             /* Specifies block count of Input Buffer */

void DspProcess(void);
void DspInit(void);

/* Definitions of buffer information structure for DMA transfer */
typedef struct buf_info
{
    int16_t * p_buf_start;                  /* Start address of buffer */
    uint16_t  samples;                      /* Samples count of a block of buffer */
} st_buf_info_t;

/* Definitions of enumerator to use to request buffer information for next DMA transfer */
typedef enum {
    NO_REQUEST   = 0x00,                    /* No request */
    DATA_REQUEST = 0x01,                    /* Requests buffer information */
} e_buf_info_request_t;


static st_buf_info_t gs_buf_info_s12ad[BUF_NUM_INFO];           /* Buffer information for DMA transfer */
static volatile e_buf_info_request_t gs_request_s12ad;          /* Request to update buffer information */
static int8_t idx_buf_s12ad = 0;


#endif