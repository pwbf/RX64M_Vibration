#include "globalVar.h"


volatile uint8_t vibrRtnRAWData[VIBR_SENS_RETURN_LENGTH];
volatile uint8_t EDSRtnRAWData[EDS_SENS_RETURN_LENGTH];

volatile int32_t frequencyData[VIBR_DATA_LENGTH / 2];
volatile int16_t vibrateData[VIBR_DATA_LENGTH];
volatile int16_t EDSData[EDS_DATA_LENGTH];

volatile uint16_t TempData[TEMP_DATA_LENGTH];

bool ETHERNET_RDY[2] = {STATE_FALSE, STATE_FALSE};