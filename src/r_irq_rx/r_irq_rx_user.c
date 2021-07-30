#include "r_cg_macrodriver.h"
#include "r_irq_rx.h"
#include "MainSetting.h"
#include "MQTT.h"
#include "EthernetProcess.h"
#include "r_cg_userdefine.h"

extern bool ProgramStatusIndicator;

/***********************************************************************************************************************
* Function Name: R_r_irq_rx_Create_UserInit
* Description  : This function adds user code after initializing the ICU module
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_r_irq_rx_Create_UserInit(void)
{
}

/***********************************************************************************************************************
* Function Name: r_r_irq_rx_irq1_interrupt
* Description  : This function is IRQ1 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_ICU_IRQ10
#pragma interrupt r_r_irq_rx_irq10_interrupt(vect=VECT(ICU,IRQ10),fint)
#else
#pragma interrupt r_r_irq_rx_irq10_interrupt(vect=VECT(ICU,IRQ10))
#endif
static void r_r_irq_rx_irq10_interrupt(void)
{
	
	printf("Terminating Service...\n");
    
	ProgramStatusIndicator = 0;
}





