/*
 * clock_setup.c
 *
 *  Created on: Mar 29, 2026
 *      Author: Rishikesh
 */


#include "stm32f411.h"
#include "clock_setup.h"


void Systick_configure(uint32_t reload_val)
{
	Systick_regdef_t* syst_ptr ;
	syst_ptr = SYST_CSR_BASEADDR ;
	syst_ptr->syst_cvr = 0 ;
	syst_ptr->syst_rvr = (reload_val-1) ;
	syst_ptr->syst_csr = ((1 << 2)| (1 << 1) | (1 << 0))  ; // Always enable the counter after setting other registers
}

void clock_set_HSE(void)
{

	RCC_RegDef_t* RCC_ptr = RCC ;
	if((RCC_ptr->CLCFGR & (3 << 2)) != (1 << 2))
	{
		RCC_ptr->CR |= (1 << 16); // Set HSE ON bit
		while(!(RCC_ptr->CR & (1 << 17) ));  // check if HSE is ready
		RCC_ptr->CLCFGR &= ~(3);
		RCC_ptr->CLCFGR |= (1 << 0); // Select HSE as system clock
		while((RCC_ptr->CLCFGR & (3 << 2)) != (1 << 2)); // wait until system clock switch status changes to HSE
	}
}


