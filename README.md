# LeadWell - Vibration  
##### Frontend(ASTARC) :  [VIBRATION ASTARC](http://vibr.astarc.tk/)  
##### Frontend(Leadwell) :  [VIBRATION Leadwell](http://210.66.194.19)  
### Receiver Server: 210.66.194.19  
### Database Server: 210.66.194.19  

## Current Function Information:  
> ## leadwellVibr.master.current:  
>> Version: leadwellVibr.master.v1.2.4b  
- [x] Ethernet  
- [x] TCP  
- [x] MQTT  
- [x] 1ch Vibration(UART)  
- [x] 1ch Temperature(ADC)  
- [x] 1ch Eddy Current Displacement(UART)  
- [x] DSP Library(FFT)  
- [x] Data Hook  
- [x] Web FrontEnd  
- [x] SQL Database  
- [x] Long term Testing  

## Highlight: 
#### EDS Fixed    
Date: 2021/11/25  
Update 1.2.4b  
Fixed: EDS byte swapping problem  

#### EDS Fixed    
Date: 2021/11/24  
Update 1.2.4a  
Fixed: EDS initial, change ID to 01  

#### EDS Fixed    
Update 1.2.2  
Fixed: Web Data Type(uint8_t -> uint16_t)  
Fixed: MQTT Data Type(uint8_t -> uint16_t)  
Added: MQTT EDS(LW0_SEDS)  

#### Embedded MQTT Client    
Update 1.2.1  
Added: Peak value in mg  
Added: Prefix for add Topic  

## Highlight: 
#### Embedded MQTT Client    
Update 1.2.0  
Added: Static RAM for vibration data  
Added: MQTT client  
Fixed: Pointer Collision  

#### Timeout indicator  
Update 1.1.5  
UART Timeout Skipper for continuous data sending  
If any sensor failed or data failed to send  
It will write into database for future debugging  

#### /src/MainSetting.h  
Update 1.1.0
You only need to change this configuration file in order to enable/disable function  
```cpp  
//src/MainSetting.h
#define		TCP_CONNID_TD		1
#define		TCP_CONNID_FD		2
#define		SRC_CLIENT_IP		0,0,0,0	
#define		TAR_SERVER_IP		172,31,37,246
#define		TAR_SERVER_PT		8120

//MODE_ENABLE | MODE_DISABLE
//Functions
#define		ENABLE_ETHERNET		(MODE_ENABLE)
#define		ENABLE_DHCP			(MODE_ENABLE)
#define		ENABLE_UART			(MODE_DISABLE)
#define		ENABLE_VIBR			(MODE_DISABLE)
#define		ENABLE_EDS			(MODE_DISABLE)
#define		ENABLE_DSP			(MODE_DISABLE)
#define		ENABLE_ADC			(MODE_ENABLE)

//Mode
#define		FLUSH_BUF_AFTER_SENDING		(MODE_DISABLE)
// 0 for normal water, 1 for sea water
``` 