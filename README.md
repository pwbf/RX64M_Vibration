# LeadWell - Vibration  
##### Frontend :  [VIBRATION ASTARC](http://vibr.astarc.tk/)  
##### Branches :  
- branch-master : Project Main(master) trunk  
- branch-VS : Vibration Sensor  
- branch-ET : Ethernet  
- branch-ET_VT : Ethernet + Vibration  
- branch-ECD : Eddy Current Displacement  
- branch-TEMP : Temperature  

## Current Function Information:  
> ## leadwellVibr.master.current:  
>> Version: leadwellVibr.master.v1.1.6  
- [x] Ethernet  
- [x] 1ch Vibration(UART)  
- [x] 1ch Temperature(ADC)  
- [ ] 1ch Eddy Current Displacement(UART)  
- [x] DSP Library(FFT)  
- [x] Data Hook  
- [x] Web FrontEnd  
- [x] SQL Database  
> ## leadwellVibr.ECD.current:  
>> Version: leadwellVibr.ECD.NONEXISTENT  
- [x] UART  
- [ ] Displacment Sensing  
- [ ] Long term Testing  
> ## leadwellVibr.VS.current:  
>> Version: leadwellVibr.VS.v1.0.2  
- [x] UART  
- [x] Vibration Sensing  
- [x] DSP FFT > Time Domain > Frequency Domain  
- [x] Long term Testing  
> ## leadwellVibr.ET.current:  
>> Version: leadwellVibr.ET.v1.0.4  
- [x]  Single Sending  
- [x]  Continuous Sending(VIBR)  
- [x]  Continuous Sending(FREQ)  
- [ ]  Continuous Sending(EDA)  
- [x]  Continuous Sending(TEMP)  
- [ ]  UDP  
- [x]  Data Hook  
- [x]  Packing  
- [ ]  NotNecessary > DNS  
- [x]  NotNecessary > DHCP  
> ## leadwellVibr.TMP.current:  
>> Version: leadwellVibr.ET.v1.0.1  
- [x]  ADC Read  

## Highlight:  

#### Update 1.1.6  
###### Debug Message Enabler  
```cpp
#define		PRINT_DEBUGGING_MESSAGE		(MODE_DISABLE)
```  
###### Wait for TCP connection  
```cpp
while(tcp_connection_status != 0 && tcp_connection_status != -57){
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf("        Waiting for Server Respond...\n");
		#endif
		R_BSP_SoftwareDelay (500, BSP_DELAY_MILLISECS);
		tcp_connection_status = TCP_Connect(encodeIPv4(TAR_SERVER_IP), TAR_SERVER_PT, TCP_CONNID_TD);
	}
```  
###### Unplug and stop, Plus and play  
```cpp
while(!ETHERNET_RDY[0]){
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf("        Waiting for Plug in signal...\n");
		#endif
		R_ETHER_LinkProcess(0);
		R_BSP_SoftwareDelay (500, BSP_DELAY_MILLISECS);
	}
```  

#### Update 1.1.5  
#### Timeout indicator  
UART Timeout Skipper for continuous data sending  
If any sensor failed or data failed to send  
It will write into database for future debugging  

#### Update 1.1.0  
###### /src/MainSetting.h  
You only need to change this configuration file in order to enable/disable function  
```cpp  
//src/MainSetting.h
#define		TCP_CONNID_TD		1
#define		TCP_CONNID_FD		2
#define		SRC_CLIENT_IP		0,0,0,0	
#define		TAR_SERVER_IP		172,31,37,246
//#define		TAR_SERVER_IP		10,0,0,178
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