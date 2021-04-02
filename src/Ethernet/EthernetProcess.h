#ifndef ETHERNET_PROCESS_H
#define ETHERNET_PROCESS_H

#include "platform.h"
#include "globalVar.h"

#include "r_t4_itcpip.h"
#include "r_ether_rx_if.h"
#include "r_t4_dhcp_client_rx_if.h"
#include "r_t4_dns_client_rx_if.h"
#include "timer.h"

#include "MainSetting.h"

#define SENDING_SIZE	2048

extern uint8_t _myethaddr[][6];
extern TCPUDP_ENV tcpudp_env[];
extern uint8_t dnsaddr1[];
extern uint8_t dnsaddr2[];
extern volatile uint8_t  link_detect[ETHER_CHANNEL_MAX];
static UW tcpudp_work[ 21504 / sizeof(UW)]; // calculated by W tcpudp_get_ramsize( void )

void set_tcpudp_env(DHCP *dhcp);
ER tcp_callback(ID cepid, FN fncd , VP p_parblk);

uint32_t encodeIPv4(uint8_t B1, uint8_t B2, uint8_t B3, uint8_t B4);
void decodeIPv4(uint32_t encIP, uint8_t *decIP);
void EthernetInit(bool usingDHCP);
void EthernetDHCP(void);
ER TCP_Connect(uint32_t dstIP, uint16_t portno, uint8_t connID);
ER TCP_SendingData(uint8_t connID, uint8_t *dataPtr, uint16_t dataSize);
ER TCP_Terminate(uint8_t connID);

typedef struct tcppacket
{
	T_IPV4EP src;
	T_IPV4EP dst;	
}TCP_PACKET;

#endif