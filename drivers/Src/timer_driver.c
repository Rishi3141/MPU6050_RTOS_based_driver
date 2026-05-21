/*
 * timer_driver.c
 *
 *  Created on: Mar 26, 2026
 *      Author: Rishikesh
 */

#include "stm32f411.h"


void IRQInterruptConfig(uint8_t IRQnum,uint8_t En)
{
	if(En == ENABLE)
	{
		if(IRQnum < 32)
		{
		   *NVIC_ISER0 |= (1 << IRQnum);
		}
		else if (IRQnum > 31 && IRQnum < 64)
		{
			*NVIC_ISER1 |= (1 << (IRQnum%32));
		}
		else if( IRQnum > 63 && IRQnum < 96)
		{
			*NVIC_ISER2 |= (1 << (IRQnum%64));
		}

	}

	else
	{

		if(IRQnum < 32)
			{
			   *NVIC_ICER0 |= (1 << IRQnum);
			}
			else if (IRQnum > 31 && IRQnum < 64)
			{
				*NVIC_ICER1 |= (1 << (IRQnum%32));
			}
			else if( IRQnum > 63 && IRQnum < 96)
			{
				*NVIC_ICER2 |= (1 << (IRQnum%64));
			}


	}

}
void IRQPriorityConfig(uint8_t IRQnum,uint8_t IRQpriority)
{
	 uint8_t ipr_reg_num = IRQnum/4 ; // gives register number from irq number
     uint8_t pos         = IRQnum%4 ; // gives byte position (0 to 3)
     *(INT_PRI_ADDR + ipr_reg_num) |= (IRQpriority << (8*pos + (8 - NUM_PRI_BITS)) );

}
