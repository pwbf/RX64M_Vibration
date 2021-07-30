#include "EthernetProcess.h"

TCP_PACKET *tcpPacket;
extern char* testingHostname;

uint32_t encodeIPv4(uint8_t B1, uint8_t B2, uint8_t B3, uint8_t B4){
	return B1<<24 | B2<<16 | B3<<8 | B4<<0; 
}

void decodeIPv4(uint32_t encIP, uint8_t *decIP){
	*(decIP+0) = encIP >> 24;
	*(decIP+1) = encIP >> 16;
	*(decIP+2) = encIP >> 8;
	*(decIP+3) = encIP >> 0;
}

void showRDATA(uint8_t *rdataPtr){
	printf("RDATA: \n");
	int loop = 0;
	char ch = rdataPtr[loop];
	while(ch){
		ch = rdataPtr[loop++];
		// printf("%c(0x%X) ", ch, ch);
		printf("%c", ch);
	}
	printf("Total Length: %d\n", loop - 1);
}
void flushRDATA(uint8_t *rdataPtr){
	//printf("Flushing RDATA: \n");
	for(int loop = 0; loop < RECEIVE_SIZE ; loop++){
		rdataPtr[loop] = 0x00;
	}
}

ER TCP_Disconnect(uint8_t connID){
	int32_t rtn = 0x00;
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf(">>> Request TCP disconnection connID = %d\n",connID);
	#endif
	
	rtn = tcp_sht_cep(connID);
	rtn = tcp_cls_cep(connID, TMO_FEVR);
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf(">>> Disconnected\n");
	#endif
	
	return rtn;
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
	
	rtn = tcp_con_cep(connID, (&tcpPacket->src), (&tcpPacket->dst), TMO_FEVR);
	
	return rtn;
}

ER TCP_ReceivingData(uint8_t connID, uint8_t *rdataPtr){
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		//printf(">> TCP_ReceivingData\n");
	#endif
	ER sRtn;
	sRtn = tcp_rcv_dat(connID, rdataPtr, RECEIVE_SIZE, TMO_FEVR);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		//printf("TCP_ReceivingData>>> rtn:0x%X\n",sRtn);
		//TCP_ShowError(sRtn);
	#endif
	//TCP_SendingACK(connID);
}

ER TCP_SendingData(uint8_t connID, uint8_t *dataPtr, uint16_t dataSize){
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		//printf(">> TCP_SendingData\n");
	#endif
	ER sRtn;
	sRtn = tcp_snd_dat(connID, dataPtr, dataSize, TMO_FEVR);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		//printf("TCP_SendingData>>> rtn:0x%X\n",sRtn);
		//TCP_ShowError(sRtn);
	#endif
}

ER TCP_SendingACK(uint8_t connID){
	uint8_t dataPtr = "";
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		//printf(">> TCP_SendingACK\n");
	#endif
	ER sRtn;
	sRtn = tcp_snd_dat(connID, &dataPtr, 1, TMO_FEVR);
	R_BSP_SoftwareDelay (100, BSP_DELAY_MILLISECS);
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		//printf("TCP_SendingACK>>> rtn:0x%X\n",sRtn);
		//TCP_ShowError(sRtn);
	#endif
}

ER TCP_Terminate(uint8_t connID){
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf(">> Terminate TCP connection\n");
	#endif
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
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("-- INIT: DNS Client\n");
	#endif
	
	R_dns_init();
	/*
	if(R_dns_resolve_name(testingHostname,&R_dns_process) != E_OK){
		printf(">>>>DNS query test FAILED\n");
		while(1);
	}
	else{
		printf(">>>>DNS query test SUCCEED\n");
		printf(">>>>Query: ");
		printf(testingHostname);
	}
	*/
	/*Check TCPIP is on*/
	//EthernetCheckTCPIP();
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
	LED_IP_ST	= LED_ON;
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("\n==========DHCP==========\n");
	#endif
        memcpy(tcpudp_env[0].ipaddr, dhcp->ipaddr, 4);
        memcpy(tcpudp_env[0].maskaddr, dhcp->maskaddr, 4);
        memcpy(tcpudp_env[0].gwaddr, dhcp->gwaddr, 4);
        memcpy(tcpudp_env[0].dnsaddr, dhcp->dnsaddr, 4);
        memcpy(tcpudp_env[0].dnsaddr2, dhcp->dnsaddr2, 4);
        memcpy((char *)dnsaddr1, (char *)dhcp->dnsaddr, 4);
        memcpy((char *)dnsaddr2, (char *)dhcp->dnsaddr2, 4);
	#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE
		printf("DHCP->IP: %d.%d.%d.%d\n",
			tcpudp_env[0].ipaddr[0], tcpudp_env[0].ipaddr[1], tcpudp_env[0].ipaddr[2], tcpudp_env[0].ipaddr[3]);
		printf("DHCP->MASK: %d.%d.%d.%d\n",
			tcpudp_env[0].maskaddr[0], tcpudp_env[0].maskaddr[1], tcpudp_env[0].maskaddr[2], tcpudp_env[0].maskaddr[3]);
		printf("DHCP->Gateway: %d.%d.%d.%d\n",
			tcpudp_env[0].gwaddr[0], tcpudp_env[0].gwaddr[1], tcpudp_env[0].gwaddr[2], tcpudp_env[0].gwaddr[3]);
		printf("DHCP->DNS1: %d.%d.%d.%d\n",
			tcpudp_env[0].dnsaddr[0], tcpudp_env[0].dnsaddr[1], tcpudp_env[0].dnsaddr[2], tcpudp_env[0].dnsaddr[3]);
		printf("DHCP->DNS2: %d.%d.%d.%d\n",
			tcpudp_env[0].dnsaddr2[0], tcpudp_env[0].dnsaddr2[1], tcpudp_env[0].dnsaddr2[2], tcpudp_env[0].dnsaddr2[3]);
	#endif

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

void TCP_ShowError(ER errorcode){
	switch (errorcode){
		case E_PAR:
			printf("TCP_DebugError>> E_PAR\n");
			break;
		case E_QOVR:
			printf("TCP_DebugError>> E_QOVR\n");
			break;
		case E_OBJ:
			printf("TCP_DebugError>> E_OBJ\n");
			break;
		case E_TMOUT:
			printf("TCP_DebugError>> E_TMOUT\n");
			break;
		default:
			printf("TCP_DebugError>> E_OK\n");
			break;
	}
}

ER tcp_callback(ID cepid, FN fncd , VP p_parblk){
	
    ER   parblk = *(ER *)p_parblk;
    ER   ercd;
    ID   cepid_oft;
    
#if PRINT_DEBUGGING_MESSAGE == MODE_ENABLE

#endif
    return(0);
}

ER ether_dns_callback(ID cepid, FN fncd , VP p_parblk)
{
    ER parblk;
    ER ercd;

    parblk = *(ER *)p_parblk;
    ercd   = E_OK;

    return (0);
}