#include "EthernetProcess.h"

TCP_PACKET *tcpPacket;

extern volatile int16_t EDSData[EDS_DATA_LENGTH];
extern volatile int16_t vibrateData[VIBR_DATA_LENGTH];
extern volatile int32_t frequencyData[VIBR_DATA_LENGTH / 2];
extern volatile uint8_t rtnRAWData[VIBR_SENS_RETURN_LENGTH];

uint32_t encodeIPv4(uint8_t B1, uint8_t B2, uint8_t B3, uint8_t B4){
	return B1<<24 | B2<<16 | B3<<8 | B4<<0; 
}

void decodeIPv4(uint32_t encIP, uint8_t *decIP){
	*(decIP+0) = encIP >> 24;
	*(decIP+1) = encIP >> 16;
	*(decIP+2) = encIP >> 8;
	*(decIP+3) = encIP >> 0;
}

ER TCP_Connect(uint32_t dstIP, uint16_t portno, uint8_t connID){
	int32_t rtn = 0x00;
	uint8_t dstdeIP[4] = {0,0,0,0};
	tcpPacket->dst.ipaddr = dstIP;
	tcpPacket->dst.portno = portno;
	
	decodeIPv4(dstIP, dstdeIP);
	
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf(">>> Request TCP connection connID = %d\n",connID);
		printf(">> Target ip=%d.%d.%d.%d:%d\n",dstdeIP[0],dstdeIP[1],dstdeIP[2],dstdeIP[3], tcpPacket->dst.portno);
	#endif
	
	rtn = tcp_con_cep(connID, (&tcpPacket->src), (&tcpPacket->dst), TMO_NBLK);
	
	return rtn;
}

ER TCP_SendingData(uint8_t connID, uint8_t *dataPtr, uint16_t dataSize){
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf(">> TCP_SendingData\n");
	#endif
	uint16_t sRtn;
	sRtn = tcp_snd_dat(connID, dataPtr, dataSize, TMO_FEVR);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("TCP_SendingData>>> rtn:%d\n",sRtn);
	#endif
}

ER TCP_Terminate(uint8_t connID){
	tcp_sht_cep(connID);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
	tcp_cls_cep(connID, TMO_FEVR);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
}

void EthernetDHCP(void){
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf(">> DHCP Initial\n");
	#endif
	volatile DHCP       dhcp;
	
	OpenTimer();
	while (ETHER_FLAG_ON_LINK_ON != link_detect[0] && ETHER_FLAG_ON_LINK_ON != link_detect[1])
	{
		R_ETHER_LinkProcess(0);
		//R_ETHER_LinkProcess(1);
	}
	
	int32_t dhcpRtn = 1;
	while(dhcpRtn){
		dhcpRtn = r_dhcp_open(&dhcp, (unsigned char*)tcpudp_work, &_myethaddr[0][0]);
		if(!dhcpRtn){
			set_tcpudp_env(&dhcp);
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">>> DHCP Process Succeed\n");
			#endif
			break;
		}
		else{
			#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
				printf(">>> DHCP Process Failed\n");
			#endif
		}
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf(">> r_dhcp_open retrying...\n");
		#endif
		R_BSP_SoftwareDelay(2, BSP_DELAY_SECS);
	}
	CloseTimer();

	if (NULL != &dhcp)
	{
		nop();
	}
}

void EthernetInit(bool usingDHCP){
	ER          ercd;
	W           ramsize;

	/* Initialize the Ethernet driver */
	R_ETHER_Initial();

	/* start LAN controller */
	ercd = lan_open();
	if (ercd != E_OK){
		for( ;; );
	}
	
	if(usingDHCP == 1){
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf("-- INIT: Using DHCP\n");
		#endif
		EthernetDHCP();
	}
	else{
		#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
			printf("-- INIT: Using Static IP\n");
		#endif
	}
	/* Get the size of the work area used by the T4 (RAM size). */
	ramsize = tcpudp_get_ramsize();
	if (ramsize > (sizeof(tcpudp_work))){
	/* Then reserve as much memory array for the work area as the size 
	   indicated by the returned value. */
		for( ;; );
	}

	/* Initialize the TCP/IP */
	ercd = tcpudp_open(tcpudp_work);
	if (ercd != E_OK){
		for( ;; );
	}
	/* Init DNS clienr */
	// R_dns_init();
}

/******************************************************************************
Function Name   : set_tcpudp_env
Description     : Set DHCP's error logs to TCP event buffer.
Arguments       : none
Return value    : none
******************************************************************************/
void set_tcpudp_env(DHCP *dhcp){
    if (NULL != dhcp)
    {
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("\n==========DHCP==========\n");
	#endif
        memcpy(tcpudp_env[0].ipaddr, dhcp->ipaddr, 4);
        memcpy(tcpudp_env[0].maskaddr, dhcp->maskaddr, 4);
        memcpy(tcpudp_env[0].gwaddr, dhcp->gwaddr, 4);
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("DHCP->IP: %d.%d.%d.%d\n",
			tcpudp_env[0].ipaddr[0], tcpudp_env[0].ipaddr[1], tcpudp_env[0].ipaddr[2], tcpudp_env[0].ipaddr[3]);
		printf("DHCP->MASK: %d.%d.%d.%d\n",
			tcpudp_env[0].maskaddr[0], tcpudp_env[0].maskaddr[1], tcpudp_env[0].maskaddr[2], tcpudp_env[0].maskaddr[3]);
		printf("DHCP->Gateway: %d.%d.%d.%d\n",
			tcpudp_env[0].gwaddr[0], tcpudp_env[0].gwaddr[1], tcpudp_env[0].gwaddr[2], tcpudp_env[0].gwaddr[3]);
	#endif

        memcpy((char *)dnsaddr1, (char *)dhcp->dnsaddr, 4);
        memcpy((char *)dnsaddr2, (char *)dhcp->dnsaddr2, 4);
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("==========DHCP==========\n\n");
	#endif
    }
    else{
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
    		printf("DHCP no reply!\n");
	#endif
    }
}

ER tcp_callback(ID cepid, FN fncd , VP p_parblk){
	
    ER   parblk = *(ER *)p_parblk;
    ER   ercd;
    ID   cepid_oft;
    
/*#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
	switch (fncd){
		case TFN_TCP_CRE_REP:
			printf("tcp_callback>> TFN_TCP_CRE_REP\n");
			break;
		case TFN_TCP_DEL_REP:
			printf("tcp_callback>> TFN_TCP_DEL_REP\n");
			break;
		case TFN_TCP_CRE_CEP:
			printf("tcp_callback>> TFN_TCP_CRE_CEP\n");
			break;
		case TFN_TCP_DEL_CEP:
			printf("tcp_callback>> TFN_TCP_DEL_CEP\n");
			break;
		case TFN_TCP_ACP_CEP:
			printf("tcp_callback>> TFN_TCP_ACP_CEP\n");
			break;
		case TFN_TCP_CON_CEP:
			printf("tcp_callback>> TFN_TCP_CON_CEP\n");
			printf("TCP [ACK] or [PSH, ACK]\n");
			break;
		case TFN_TCP_SHT_CEP:
			printf("tcp_callback>> TFN_TCP_SHT_CEP\n");
			printf(">> TCP data transmission end\n");
			break;
		case TFN_TCP_CLS_CEP:
			printf("tcp_callback>> TFN_TCP_CLS_CEP\n");
			
			break;
		case TFN_TCP_SND_DAT:
			printf("tcp_callback>> TFN_TCP_SND_DAT\n");
			printf("Transmission of TCP data\n");
			break;
		case TFN_TCP_RCV_DAT:
			printf("tcp_callback>> TFN_TCP_RCV_DAT\n");
			printf("Reception of TCP data\n");
			break;
		case TFN_TCP_GET_BUF:
			printf("tcp_callback>> TFN_TCP_GET_BUF\n");
			break;
		case TFN_TCP_SND_BUF:
			printf("tcp_callback>> TFN_TCP_SND_BUF\n");
			break;
		case TFN_TCP_RCV_BUF:
			printf("tcp_callback>> TFN_TCP_RCV_BUF\n");
			break;
		case TFN_TCP_REL_BUF:
			printf("tcp_callback>> TFN_TCP_REL_BUF\n");
			break;
		case TFN_TCP_SND_OOB:
			printf("tcp_callback>> TFN_TCP_SND_OOB\n");
			break;
		case TFN_TCP_RCV_OOB:
			printf("tcp_callback>> TFN_TCP_RCV_OOB\n");
			break;
		case TFN_TCP_CAN_CEP:
			printf("tcp_callback>> TFN_TCP_CAN_CEP\n");
			break;
		case TFN_TCP_SET_OPT:
			printf("tcp_callback>> TFN_TCP_SET_OPT\n");
			break;
		case TFN_TCP_GET_OPT:
			printf("tcp_callback>> TFN_TCP_GET_OPT\n");
			break;
		case TFN_TCP_ALL:
			printf("tcp_callback>> 0\n");
			break;
		case TFN_UDP_CRE_CEP:
			printf("tcp_callback>> TFN_UDP_CRE_CEP\n");
			break;
		case TFN_UDP_DEL_CEP:
			printf("tcp_callback>> TFN_UDP_DEL_CEP\n");
			break;
		case TFN_UDP_SND_DAT:
			printf("tcp_callback>> TFN_UDP_SND_DAT\n");
			break;
		case TFN_UDP_RCV_DAT:
			printf("tcp_callback>> TFN_UDP_RCV_DAT\n");
			break;
		case TFN_UDP_CAN_CEP:
			printf("tcp_callback>> TFN_UDP_CAN_CEP\n");
			break;
		case TFN_UDP_SET_OPT:
			printf("tcp_callback>> TFN_UDP_SET_OPT\n");
			break;
		case TFN_UDP_GET_OPT:
			printf("tcp_callback>> TFN_UDP_GET_OPT\n");
			break;
		default:
			printf("tcp_callback>> NOPE\n");
			break;
	}
#endif*/
    return(0);
}