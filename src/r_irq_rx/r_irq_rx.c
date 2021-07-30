/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_irq_rx.c
* Version      : 2.0.1
* Device(s)    : R5F564MLCxFC
* Description  : This file implements device driver for r_irq_rx.
* Creation Date: 2020-07-21
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_irq_rx.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_r_irq_rx_Create
* Description  : This function initializes the ICU module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_r_irq_rx_Create(void)
{
    /* Disable IRQ interrupts */
    ICU.IER[0x08].BYTE = 0x00U;
    ICU.IER[0x09].BYTE = 0x00U;

    /* Disable software interrupt */
    IEN(ICU,SWINT) = 0U;
    IEN(ICU,SWINT2) = 0U;

    /* Disable IRQ digital filter */
    ICU.IRQFLTE0.BYTE &= ~(_04_ICU_IRQ10_FILTER_ENABLE);

    /* Set IRQ detection type */
    ICU.IRQCR[10].BYTE = _04_ICU_IRQ_EDGE_FALLING;

    /* Set IRQ1 priority level */
    IPR(ICU,IRQ10) = _0F_ICU_PRIORITY_LEVEL15;

    /* Set IRQ1 pin */
    MPC.P02PFS.BYTE = 0x40U;
    PORT0.PDR.BYTE &= 0xFDU;
    PORT0.PMR.BYTE &= 0xFDU;

    R_r_irq_rx_Create_UserInit();
    
}

/***********************************************************************************************************************
* Function Name: R_r_irq_rx_IRQ10_Start
* Description  : This function enables IRQ1 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_r_irq_rx_IRQ10_Start(void)
{
    /* Enable IRQ10 interrupt */
    IEN(ICU,IRQ10) = 1U;
}

/***********************************************************************************************************************
* Function Name: R_r_irq_rx_IRQ10_Stop
* Description  : This function disables IRQ10 interrupt
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_r_irq_rx_IRQ10_Stop(void)
{
    /* Disable IRQ1 interrupt */
    IEN(ICU,IRQ10) = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */   

