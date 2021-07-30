#ifndef MISC_CLAC_H
#define MISC_CLAC_H


#include <math.h>

#include "platform.h"
#include "globalVar.h"

#include "MainSetting.h"

void arrayAverage(uint8_t * ary, uint16_t len);
void arrayMaximumFD(uint8_t * ary, uint16_t len);
void arrayMaximumTD(int16_t  * virb_feed, uint16_t len);
void arrayRMS(int16_t  * virb_feed, uint16_t len);

#endif