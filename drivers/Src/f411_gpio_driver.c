/*
 * f411_gpio_driver.c
 *
 *  Created on: Aug 26, 2025
 *      Author: Rishikesh
 */
//#include "stm32f411.h"   // driver source file should contain driver header file
#include "f411_gpio_driver.h"

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
   uint32_t var = 0 ;
   uint32_t mask ;

   GPIO_ClkCtrl(pGPIOHandle->pGPIO_Base_Addr, ENABLE);

	// configure the mode of gpio pin
  if(pGPIOHandle->pinConfig.GPIO_PinMode  <= GPIO_MODE_ANALOG)    // when pin mode is non interrupt type
  {

	var = (pGPIOHandle->pinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->pinConfig.GPIO_PinNumber) );
	mask = ~(0x00000003 << (2* pGPIOHandle->pinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIO_Base_Addr->PUPDR &= mask ;   //
	pGPIOHandle->pGPIO_Base_Addr->PUPDR |= var   ;

   if(pGPIOHandle->pinConfig.GPIO_PinMode == GPIO_MODE_OUT || pGPIOHandle->pinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		var = (pGPIOHandle->pinConfig.GPIO_PinOPType << (pGPIOHandle->pinConfig.GPIO_PinNumber) );
		mask = ~(0x00000001 << ( pGPIOHandle->pinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIO_Base_Addr->OTYPER &= mask ;   //
		pGPIOHandle->pGPIO_Base_Addr->OTYPER |= var   ;
	}

    var = (pGPIOHandle->pinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->pinConfig.GPIO_PinNumber) );
	mask = ~(0x00000001 << (2* pGPIOHandle->pinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIO_Base_Addr->OSPEEDR &= mask ;   //
	pGPIOHandle->pGPIO_Base_Addr->OSPEEDR |= var   ;




   if(pGPIOHandle->pinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
   {
  // configure alternate function registers

	   uint8_t tem1,tem2;
	   tem1 = pGPIOHandle->pinConfig.GPIO_PinNumber / 8 ; // to decide AFR[0] or AFR[1]
	   tem2 = pGPIOHandle->pinConfig.GPIO_PinNumber % 8 ; // to decide no. of position for bitshifting

	//   pGPIOHandle->pGPIO_Base_Addr->AFR[tem1] &= ~(0x0000000F << (4*tem2));
	//   pGPIOHandle->pGPIO_Base_Addr->AFR[tem1] |= (pGPIOHandle->pinConfig.GPIO_PinAltFunMode  << (4* tem2) );

	   pGPIOHandle->pGPIO_Base_Addr->AFR[tem1] &= ~(0xF << ( 4 * tem2 ) ); //clearing
	   pGPIOHandle->pGPIO_Base_Addr->AFR[tem1] |= (pGPIOHandle->pinConfig.GPIO_PinAltFunMode << ( 4 * tem2 ) );

   }

   var = (pGPIOHandle->pinConfig.GPIO_PinMode << (2* pGPIOHandle->pinConfig.GPIO_PinNumber) );
   mask = ~(0x00000003 << (2* pGPIOHandle->pinConfig.GPIO_PinNumber));
   pGPIOHandle->pGPIO_Base_Addr->MODER &= mask ;   //
   pGPIOHandle->pGPIO_Base_Addr->MODER |= var  ;



  }

  else
  {
	  // when gpio mode is interrupt
	   if(pGPIOHandle->pinConfig.GPIO_PinMode == GPIO_MODE_IT_FL)
	  {
          EXTI->FTSR |= (1 << pGPIOHandle->pinConfig.GPIO_PinNumber);
          EXTI->RTSR &= ~(1 << pGPIOHandle->pinConfig.GPIO_PinNumber); // clear the corresponding RTSR bit
	  }

	  else if(pGPIOHandle->pinConfig.GPIO_PinMode == GPIO_MODE_IT_RS)
	   {
          EXTI->RTSR |= (1 << pGPIOHandle->pinConfig.GPIO_PinNumber);
          EXTI->FTSR &= ~(1 << pGPIOHandle->pinConfig.GPIO_PinNumber); // clear the corresponding FTSR bit

	   }

	  else   // interrupt mode == rising and falling
	  {

	  }
	   uint8_t tem1 = (  pGPIOHandle->pinConfig.GPIO_PinNumber / 4 );  // to get EXTICR array index
	   uint8_t tem2 = (  pGPIOHandle->pinConfig.GPIO_PinNumber % 4 ); // to get no. of positions for bit shifting
	   uint8_t code = BASEADDR_TO_CODE(pGPIOHandle->pGPIO_Base_Addr);  //to map port to corresponding value to be assigned in EXTICR
	   SYSCFG->EXTICR[tem1] = (code << 4*tem2);
	   SYSCFG_CLK_EN();

	   EXTI->IMR |= (1 << pGPIOHandle->pinConfig.GPIO_PinNumber); // set the corresponding bit in interrupt mask register



  }



}


void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
  if(pGPIOx == GPIOA)
  {
	  GPIOA_REG_RESET();
  }
  else if(pGPIOx == GPIOB)
   {
	  GPIOB_REG_RESET();
   }
  else if(pGPIOx == GPIOC)
   {
	  GPIOC_REG_RESET();
   }
  else if(pGPIOx == GPIOD)
   {
	  GPIOD_REG_RESET();
   }
  else if(pGPIOx == GPIOE)
   {
	  GPIOE_REG_RESET();
   }
  else if(pGPIOx == GPIOH)
   {
	  GPIOH_REG_RESET();
   }

}


void GPIO_ClkCtrl(GPIO_RegDef_t *pGPIOx, uint8_t En)  // En_Ds means enable or disable
{
   if (En == ENABLE)
   {
	  if(pGPIOx == GPIOA)
	  {
		  GPIOA_CLK_EN();
	  }
	  else if(pGPIOx == GPIOB)
	  {
		  GPIOB_CLK_EN();
	  }
	  else if(pGPIOx == GPIOC)
	  {
		  GPIOC_CLK_EN();
	  }
	  else if(pGPIOx == GPIOD)
	  {
		  GPIOD_CLK_EN();
	  }
	  else if(pGPIOx == GPIOE)
	  {
		  GPIOE_CLK_EN();
	  }
	  else   // GPIOH
	  {
		  GPIOH_CLK_EN();
	  }



   }

   else
   {
	   if(pGPIOx == GPIOA)
	  	  {
	  		  GPIOA_CLK_DS();
	  	  }
	  	  else if(pGPIOx == GPIOB)
	  	  {
	  		  GPIOB_CLK_DS();
	  	  }
	  	  else if(pGPIOx == GPIOC)
	  	  {
	  		  GPIOC_CLK_DS();
	  	  }
	  	  else if(pGPIOx == GPIOD)
	  	  {
	  		  GPIOD_CLK_DS();
	  	  }
	  	  else if(pGPIOx == GPIOE)
	  	  {
	  		  GPIOE_CLK_DS();
	  	  }
	  	  else   // GPIOH
	  	  {
	  		  GPIOH_CLK_DS();
	  	  }


   }
}


uint8_t  GPIO_ReadPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber)
{
	uint8_t input_val ;
	input_val = (pGPIOx->IDR >> PinNumber) & (0x00000001) ;
	return input_val ;
}

uint16_t GPIO_ReadPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t port_val ;
	port_val = (uint16_t)(pGPIOx->IDR);
	return port_val ;

}

void GPIO_WritePin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber, uint8_t value)
{
	if(value == SET)
	{
	pGPIOx->ODR |= (1 << PinNumber);
	}

	else
	{
     pGPIOx->ODR &= ~(1 << PinNumber);
	}
}

void GPIO_WritePort(GPIO_RegDef_t *pGPIOx ,uint16_t value)
{
	pGPIOx->ODR = value ;


}

void GPIO_Togglepin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);
}

void GPIO_IRQConfig(uint8_t IRQ_num, uint8_t En)   // To enable interrupt
{
	if(En == ENABLE)
	{
		if(IRQ_num < 32)
		{
           *NVIC_ISER0 |= (1 << IRQ_num);
		}
		else if (IRQ_num > 31 && IRQ_num < 64)
		{
			*NVIC_ISER1 |= (1 << (IRQ_num%32));
		}
		else if( IRQ_num > 63 && IRQ_num < 96)
		{
			*NVIC_ISER2 |= (1 << (IRQ_num%64));
		}

	}

	else
	{

		if(IRQ_num < 32)
			{
	           *NVIC_ICER0 |= (1 << IRQ_num);
			}
			else if (IRQ_num > 31 && IRQ_num < 64)
			{
				*NVIC_ICER1 |= (1 << (IRQ_num%32));
			}
			else if( IRQ_num > 63 && IRQ_num < 96)
			{
				*NVIC_ICER2 |= (1 << (IRQ_num%64));
			}


	}
}

// uint8_t IRQ_priority

void GPIO_IRQ_Priorityconfig(uint8_t IRQ_num, uint32_t IRQ_priority)
{
  uint8_t ipr_reg_num = IRQ_num/4 ; // gives register number from irq number
  uint8_t pos         = IRQ_num%4 ; // gives byte position (0 to 3)
  *(INT_PRI_ADDR + ipr_reg_num) |= (IRQ_priority << (8*pos + (8 - NUM_PRI_BITS)) );
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
 if(EXTI->PR & (1 << PinNumber)) // bit is set when selected edge event arrives on a selected GPIO pin
 {
	 EXTI->PR |= (1 << PinNumber);  //Clear the pending bit. (this bit is cleared by programming it to 1)
 }

}






