/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name	   : cFFT_in64_i16.h
* Description  : input data for complex FFT
******************************************************************************/
/*****************************************************************************
* History      : YYYY.MM.DD  Version  Description 
*              : 2014.01.02  4.00     Fourth version
******************************************************************************/

/******************************************************************************
Macro definitions
******************************************************************************/
#define FFT_POINTS (64)


/******************************************************************************
Exported global variables
******************************************************************************/
/* 64 points input data */
const int16_t cFFT_buf_time[2*FFT_POINTS] = {
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
0, 0,
-1, -1,
-2, -3,
-2, -1,
1, 5,
8, 10,
10, 8,
3, -2,
-8, -12,
-13, -11,
-8, -4,
-1, -1,
-2, -4,
-3, 0,
7, 18,
28, 35,
35, 26,
8, -13,
-34, -48,
-53, -46,
-30, -10,
8, 22,
27, 25,
19, 13,
11, 15,
21, 27,
28, 21,
4, -17,
-40, -57,
-63, -55,
-35, -9,
17, 37,
47, 46,
36, 21,
7, -2,
-6, -5,
-3, -2,
-3, -8,
-15, -21,
-23, -20,
-13, -2,
8, 17,
21, 21,
16, 9,
1, -3,
-7, -7,
-6, -4,
-2, -1,
0, -1,
-1, -1,
0, 0,
0, 1,
1, 1,
1, 0,
0, 0,
0, 0,
0, 0,
0, 0
};

