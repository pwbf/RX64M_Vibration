/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2014 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_ether_rx_config.h
* Version      : 1.00
* Description  : Ethernet module device driver
***********************************************************************************************************************/
/**********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 22.07.2014 1.00     First Release
***********************************************************************************************************************/

/* Guards against multiple inclusion */
#ifndef R_ETHER_RX_CONFIG_H
#define R_ETHER_RX_CONFIG_H

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

/* Ethernet interface select
   0 = MII  (Media Independent Interface)
   1 = RMII (Reduced Media Independent Interface)
*/
#define ETHER_CFG_MODE_SEL              (0)

/* PHY-LSI address setting for ETHER0/1 */
#define ETHER_CFG_CH0_PHY_ADDRESS       (0)     /* Please define the PHY-LSI address in the range of 0-15. */
#define ETHER_CFG_CH1_PHY_ADDRESS       (1)     /* Please define the PHY-LSI address in the range of 0-15. */

/* The number of Rx descriptors. */
#define ETHER_CFG_EMAC_RX_DESCRIPTORS   (1)

/* The number of Tx descriptors. */
#define ETHER_CFG_EMAC_TX_DESCRIPTORS   (1)

/* Please define the size of the sending and receiving buffer in the value where one frame can surely be stored 
   because the driver is single-frame/single-buffer processing.  */
#define ETHER_CFG_BUFSIZE               (1536)  /* Must be 32-byte aligned */

/* Group AL1 interrupt priority level */
#define ETHER_CFG_AL1_INT_PRIORTY       (2)     /* Please define the interruption level within the range of 1-15. */

/* The register bus of PHY0/1 for ETHER0/1 select
   0 = The access of the register of PHY uses ETHER0. 
   1 = The access of the register of PHY uses ETHER1.
*/
#define ETHER_CFG_CH0_PHY_ACCESS        (0)
#define ETHER_CFG_CH1_PHY_ACCESS        (0)

/* Define the access timing of MII/RMII register */
#define ETHER_CFG_PHY_MII_WAIT          (8)     /* Plese define the value of 8 or more */

/* Define the waiting time for reset completion of PHY-LSI */
#define ETHER_CFG_PHY_DELAY_RESET       (0x00020000L)

/**
 * Link status read from LMON bit of ETHERC PSR register.  The state is hardware dependent.
 */
#define ETHER_CFG_LINK_PRESENT          (0)

/* Definition of whether or not to use KSZ8041NL of the Micrel Inc.
   0 = unused
   1 = use
*/
#define ETHER_CFG_USE_PHY_KSZ8041NL     (0)


/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/

#endif /* R_ETHER_RX_CONFIG_H */
