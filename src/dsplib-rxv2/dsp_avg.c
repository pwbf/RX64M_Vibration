/*
 * dsp_avg.c
 *
 *  Created on: 2019年4月8日
 *      Author: Administrator
 */

#include <stdio.h>
//version 1.0.4
/*
	add inverse negative number
*/
//int main()
//float data[10]={1,2,3,4,5,6,7,8,9,10};

unsigned int findavg(short int * ary)
{
    int i;
    unsigned int sum = 0.0, average;
    //num should be the input of data
    //n is amount data of num
    for(i = 0; i < 1024; ++i)
    {

        sum += abs(ary[i]);
    }

    average = sum / 1024;
    return average;
}

