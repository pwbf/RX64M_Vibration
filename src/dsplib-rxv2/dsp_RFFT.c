#if __FPU == 1
/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "dsp_header.h"

#include "r_dsp_transform.h"
#include "rFFT_in256_f32.h"
#include "windowCoefficient_f32.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#define FFT_POINTS 	(1024)
#define FFT_LOGN	(4)
#define FFT_NUM_BITREV  (240)
/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
 
/******************************************************************************
Private global variables and functions
******************************************************************************/
static cplxf32_t buf_freq[FFT_POINTS/2];
static float buf_inverse[FFT_POINTS];

/* rFFT_buf_time is an float type array. */
static vector_t vtime = {FFT_POINTS, (void *)rFFT_buf_time};

/* buf_freq is an float type array. */
static vector_t vfreq = {FFT_POINTS/2, (void *)buf_freq}; // half the samples if treated as complex.

/* buf_inverse is an float type array. */
static vector_t vout = {FFT_POINTS, (void *)buf_inverse};

/* initialize portion of handle structure */
static r_dsp_fft_t h = {FFT_POINTS, 0 /* default options set */};

/* initialize required buffer sizes for twiddles etc. */
/* NOTE real-valued input specified! */
static size_t ntwb; // # bytes for twiddle array
static size_t nbrb; // # bytes for bit-reverse table
static size_t nwkb; // # bytes for work area

static cplxf32_t fft_twiddles[(FFT_POINTS/2) + (FFT_POINTS/4)];
static int32_t fft_bitrev[((((FFT_POINTS>>1) - (1<<(FFT_LOGN>>1)) - 2) >> 1) + 1)];

void R_DSP_REAL_FFT_Init(void);
void R_DSP_REAL_FFT_Operation(int16_t *in_buf, int32_t *out_buf);

static cplxi32_t    gs_fft_buf[(FFT_POINTS/2)];                     /* Buffer to store R_DSP_FFT_i16ci32 result */
static vector_t     gs_fft_time = {FFT_POINTS, NULL};               /* Input buffer information
                                                                        for R_DSP_FFT_i16ci32 */
static vector_t     gs_fft_freq = {(FFT_POINTS/2), gs_fft_buf};     /* Output buffer information
                                                                        for R_DSP_FFT_i16ci32 */
static vector_t     gs_fft_mag  = {(FFT_POINTS/2), NULL};           /* Output buffer information 
                                                                        for R_DSP_VecCplxMag_ci32i32 */
static r_dsp_fft_t  gs_fft_handle = {FFT_POINTS, 0};                /* FFT Handle Data, default options */
static int16_t      gs_fft_twiddles[(FFT_POINTS+(FFT_POINTS/2))];   /* twiddle factors: 3072bytes needed */
static int32_t      gs_fft_bitrev[FFT_NUM_BITREV];                  /* bit reversal tables */

/******************************************************************************
* Function Name: dsp_realFFT
* Description  : Sample code to demonstrate real FFT
* Arguments    : none
* Return Value : status
******************************************************************************/
r_dsp_status_t dsp_realFFT(void){
    r_dsp_status_t status = R_DSP_STATUS_OK;
    
    status = R_DSP_FFT_BufSize_f32cf32(&h, &ntwb, &nbrb, &nwkb);
    if (R_DSP_STATUS_OK != status)
    {
        return status;
    }    
    else {
        if (ntwb < (sizeof(fft_twiddles)))
        {
				printf("array size is not enough(ntwb)\n");
            while(1)
            {
                /* array size is not enough. */
                ;
            }
        }
        if (nbrb < (sizeof(fft_bitrev)))
        {
				//printf("array size is not enough(nbrb)\n");
            while(1)
            {
                /* array size is not enough. */
                ;
            }
        }
    }

    /* now that we have the buffer sizes, allocate working buffers for reference */
    /*
     h.twiddles = ntwb ? malloc(ntwb) : NULL;
     h.bitrev = nbrb ? malloc(nbrb) : NULL;
     h.work = nwkb ? malloc(nbrb) : NULL;
    */
    h.n = FFT_POINTS;
    h.work = NULL;
    h.options = 0;

    /* fft_bitrev is an int32_t type array. */
    h.bitrev = (void *)fft_bitrev;

    /* set R_DSP_FFT_OPT_NO_BITREV if you don't need bit-reverse like h.options |= R_DSP_FFT_OPT_NO_BITREV */
    h.twiddles = (void *)fft_twiddles;

    /* set NULL if you don't need windowing */
    h.window = (void *)rfft_tbl_window;

    /* now we can complete initialization of the handle */
    /* NOTE real-valued input specified in the function name */
    status = R_DSP_FFT_Init_f32cf32(&h);
    if (R_DSP_STATUS_OK != status)
    {
        return status;
    }    

    status = R_DSP_FFT_f32cf32(&h, &vtime, &vfreq);
    if (R_DSP_STATUS_OK != status)
    {
        return status;
    }    
    
    h.n = FFT_POINTS;
    h.window = NULL; // Do not refer in Inverse Complex-Conjugate-Symmetric Inverse FFT

    /* now re-initialization of the handle */
    status = R_DSP_FFT_Init_cf32f32(&h);
    if (R_DSP_STATUS_OK != status)
    {
        return status;
    }    
    
    status = R_DSP_IFFT_CCS_cf32f32(&h, &vfreq, &vout);

    /* clean up */
    /*
    if (h.work != NULL) {
        free(h.work);
    }
    */
    
    /*---------------  Output data are now ready  ---------------*/
    return status;
    
} /* End of function sample_dsp_realFFT() */
#endif /* __FPU */

/******************************************************************************
 * Function Name: R_DSP_REAL_FFT_Init
 * Description  : Initializes FFT processing.
 * Arguments    : none
 * Return Value : none
 ******************************************************************************/
void R_DSP_REAL_FFT_Init(void){
    /* Initializes FFT handle data */
    gs_fft_handle.n        = FFT_POINTS;
    gs_fft_handle.work     = NULL;
    gs_fft_handle.options  = (R_DSP_FFT_OPT_SCALE | R_DSP_FFT_BIT_REVERSAL_DEFAULT | R_DSP_FFT_OPT_TWIDDLE_DEFAULT);
    gs_fft_handle.bitrev   = gs_fft_bitrev;
    gs_fft_handle.twiddles = gs_fft_twiddles;
    gs_fft_handle.window   = NULL;

    /* Initializes FFT before using R_DSP_FFT_i16ci32 */
    if (R_DSP_STATUS_OK != R_DSP_FFT_Init_i16ci32(&gs_fft_handle))
    {
        while (1)
        {
				//printf("R_DSP_STATUS_OK != R_DSP_FFT_Init_i16ci32\n");
            //nop(); /* no operation */
        }
    }    
} /* End of function R_DSP_REAL_FFT_Init() */

/******************************************************************************
 * Function Name: R_DSP_REAL_FFT_Operation
 * Description  : Does real FFT 
 *                  and calculates the magnitude of the real FFT results.
 * Arguments    : in_buf -
 *                  Pointer of where to get the input data.
 *                out_buf -
 *                  Pointer of where to put the output data.
 * Return Value : none
 ******************************************************************************/
void R_DSP_REAL_FFT_Operation(int16_t * in_buf, int32_t * out_buf){
	
    gs_fft_time.data = in_buf;
    gs_fft_mag.data  = out_buf;
    // gs_fft_freq.data  = out_buf;
	
    /* Executes real FFT */
    if (R_DSP_STATUS_OK != R_DSP_FFT_i16ci32(&gs_fft_handle, &gs_fft_time, &gs_fft_freq))
    {
        while (1)
        {
				//printf("R_DSP_STATUS_OK != R_DSP_FFT_i16ci32\n");
            //nop(); /* no operation */
        }
    }

    /*** Calculation to get magnitudes of real FFT results ***/
    /* 
       According to the DSP library specification, R_DSP_FFT_i16ci32 outputs 
        512th(1024/2th) real number on gs_fft_buf[0].im. In order to get expected
        magnitudes, the effects of the number must be removed.
    */
    /* 1) Calculation to get (FFT_POINTS/2) number of magnitudes.
          Calculates magnitudes of the complex numbers output by R_DSP_FFT_i16ci32. */
    if (R_DSP_STATUS_OK != R_DSP_VecCplxMag_ci32i32(&gs_fft_freq, &gs_fft_mag, (FFT_POINTS/2)))
    {
        while (1)
        {
				//printf("R_DSP_STATUS_OK != R_DSP_VecCplxMag_ci32i32\n");
            //nop(); /* no operation */
        }
    }

    /* 2) Replacing the 0th element of the calculation results.
          Replaces the 0th element of the calculation results with gs_fft_buf[0].re
           to complete magnitudes of real FFT results.
          This processing is needed to remove effects of the 512th(1024/2th) real number. */
    *(int32_t *)gs_fft_mag.data = gs_fft_buf[0].re;     /* This processing is the same as following;
                                                            out_buf[0] = gs_fft_buf[0].re;
                                                           The gs_fft_mag.data points out_buf[0]. */
} /* End of function R_DSP_REAL_FFT_Operation() */

