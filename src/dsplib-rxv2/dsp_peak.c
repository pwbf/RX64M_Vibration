/*
 * dsp_peak.c
 *
 *  Created on: 2019年4月8日
 *      Author: Administrator
 */
/*#include "r_dsp_transform.h"
#include "windowCoefficient_f32.h"*/
#include "stdio.h"

short int find_maximum(short int * ary) {
	int c;
	short int max, index;
 
  max = ary[0];
  index = 0;
 
  for (c = 1; c < 1024; c++) {
    if (ary[c] > max) {
       index = c;
       max = ary[c];
    }
  }
 
  return ary[index];
}
