#include "globalVar.h"

/*
Average:_xxx.x(mg)
Peak:__xxx(Hz)_xxxx(mg)
RMS:_xxx.x(mg)
Temperature:_xxx.x(*c)
*/

bool ProgramStatusIndicator = 0;		//Use for while loop deciding wether should continue or not

#pragma section _MQTT_SEC
const uint8_t pfx_avg[MQTT_VAVG_PREFIX_LENGTH]	= {'A','v','e','r','a','g','e',':',' '};
const uint8_t pfx_pk[MQTT_PEAK_PREFIX_LENGTH]	= {'P','e','a','k',':',' '};
const uint8_t pfx_rms[MQTT_VRMS_PREFIX_LENGTH]	= {'R','M','S',':',' '};
const uint8_t pfx_temp[MQTT_TEMP_PREFIX_LENGTH]	= {'T','e','m','p','e','r','a','t','u','r','e',':',' '};

const uint8_t iu_avg[MQTT_STRUCT_UNIT_LENGTH]	= {'(', 'm', 'g', ')'};
const uint8_t iu_pk[MQTT_STRUCT_UNIT_LENGTH]	= {'(', 'H', 'z', ')'};
const uint8_t iu_pkval[MQTT_STRUCT_UNIT_LENGTH]	= {'(', 'm', 'g', ')'};
const uint8_t iu_rms[MQTT_STRUCT_UNIT_LENGTH]	= {'(', 'm', 'g', ')'};
const uint8_t iu_temp[MQTT_STRUCT_UNIT_LENGTH]	= {'(', '*', 'C', ')'};

const uint8_t Topic_LW0_TEMP[MQTT_TOPIC_LENGTH] = {'L', 'W', '0', '_', 'T', 'E', 'M', 'P'};
const uint8_t Topic_LW0_VRMS[MQTT_TOPIC_LENGTH] = {'L', 'W', '0', '_', 'V', 'R', 'M', 'S'};
const uint8_t Topic_LW0_VAVG[MQTT_TOPIC_LENGTH] = {'L', 'W', '0', '_', 'V', 'A', 'V', 'G'};
const uint8_t Topic_LW0_PEAK[MQTT_TOPIC_LENGTH] = {'L', 'W', '0', '_', 'P', 'E', 'A', 'K'};
const uint8_t Topic_THU_TEST[MQTT_TOPIC_LENGTH] = {'T', 'H', 'U', '_', 'T', 'E', 'S', 'T'};
const uint8_t TestMessage[MQTT_STRUCT_VALUE_LENGTH + MQTT_STRUCT_UNIT_LENGTH] = {'A'};
#pragma section 

#pragma section _VIBR_SEC
volatile uint8_t vibrRtnRAWData[VIBR_SENS_RETURN_LENGTH];
volatile uint8_t EDSRtnRAWData[EDS_SENS_RETURN_LENGTH];

volatile int32_t frequencyData[VIBR_DATA_LENGTH / 2];
volatile int16_t vibrateData[VIBR_DATA_LENGTH];
volatile int16_t EDSData[EDS_DATA_LENGTH];
volatile uint16_t TempData[TEMP_DATA_LENGTH];

volatile int8_t HLvibrateData[HL_VIBR_DATA_LENGTH];
volatile int8_t HLTempData[HL_TEMP_DATA_LENGTH];

volatile MQTT_PAYLOAD mp;
volatile VIBR_MISC vm;
volatile MQTT_VIBR mv;
#pragma section 

bool ETHERNET_RDY[2] = {STATE_FALSE, STATE_FALSE};
uint8_t DATA_RDY[DATA_RDY_IND_LENGTH] = {0xEE, STATE_FALSE, STATE_FALSE, STATE_FALSE, STATE_FALSE};
