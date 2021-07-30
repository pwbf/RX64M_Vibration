#include "MiscClac.h"

extern volatile VIBR_MISC vm;

void arrayAverage(int16_t * ary, uint16_t len){
    float sum = 0.0, avg;
    for(uint16_t i = 0; i < len; ++i){

        sum += abs(ary[i]);
    }
    avg = sum / len;
    vm.avg = avg;
}

void arrayMaximumTD(int16_t * ary, uint16_t len){
	float ary_max = ary[1];
	uint16_t index = 1;

	for (uint16_t i = 1; i < len; i++) {
		if (ary[i] > ary_max) {
			index = i;
			ary_max = ary[i];
		}
	}
	
    vm.pkval = abs((int) ary[index]);
}

void arrayMaximumFD(int32_t * ary, uint16_t len){
	int32_t ary_max = ary[1];
	uint16_t index = 1;

	for (uint16_t i = 1; i < len; i++) {
		if (ary[i] > ary_max) {
			index = i;
			ary_max = ary[i];
		}
	}
    vm.pk = index;
}

void arrayRMS(int16_t * ary, uint16_t len){
	float sum=0;
	float mean;
	float rms;
	
	for(uint16_t i = 0; i < len; i++){
		sum += ary[i] * ary[i];
	}
	
	mean = sum/len;
	rms = (double) sqrt(mean);
    vm.rms = rms;
}