/*
 * f411_gpio_driver.h
 *
 *  Created on: Aug 26, 2025
 *      Author: Rishikesh
 */

#ifndef INC_F411_GPIO_DRIVER_H_
#define INC_F411_GPIO_DRIVER_H_

#include "stm32f411.h"    // driver header file should contain MCU specific header file

typedef struct
{
 uint8_t GPIO_PinNumber ; // possible values from @GPIO_PINNUM
 uint8_t GPIO_PinMode ;  // possible values from @GPIO_PINMODES
 uint8_t GPIO_PinSpeed ;  // possible values from @GPIO_SPEED
 uint8_t GPIO_PinPuPdControl; // possible values from @GPIO_PUPD
 uint8_t GPIO_PinOPType ;    //possible values from @OPTYPE
 uint8_t GPIO_PinAltFunMode ;

}GPIO_PinConfig_t;

// user will fill in the attributes of this structure(through GPIO_Handle_t) and give it to
// GPIO_RegDef_t defined in MCU specific header

typedef struct
{
	 GPIO_RegDef_t *pGPIO_Base_Addr ;
     GPIO_PinConfig_t pinConfig ;

}GPIO_Handle_t;






#define BASEADDR_TO_CODE(x)    ( (x==GPIOA)? 0:\
		                         (x==GPIOB)? 1:\
		                         (x==GPIOC)? 2:\
                                 (x==GPIOD)? 3:\
                                 (x==GPIOE)? 4:\
                                 (x==GPIOH)? 7:0)


//GPIO_Handle_t abc ;


// @GPIO_PINNUM
#define GPIO_PIN_0   0
#define GPIO_PIN_1   1
#define GPIO_PIN_2   2
#define GPIO_PIN_3   3
#define GPIO_PIN_4   4
#define GPIO_PIN_5   5
#define GPIO_PIN_6   6
#define GPIO_PIN_7   7
#define GPIO_PIN_8   8
#define GPIO_PIN_9   9
#define GPIO_PIN_10  10
#define GPIO_PIN_11  11
#define GPIO_PIN_12  12
#define GPIO_PIN_13  13
#define GPIO_PIN_14  14


// @GPIO_PINMODES
#define GPIO_MODE_IN      0
#define GPIO_MODE_OUT     1
#define GPIO_MODE_ALTFN   2
#define GPIO_MODE_ANALOG  3    // non interrupt modes above
#define GPIO_MODE_IT_FL   4   // interrupt, falling edge
#define GPIO_MODE_IT_RS   5  // interrupt, rising edge
#define GPIO_MODE_IT_RSFL 6   // interrupt, rising and falling edge

//output type :  @OPTYPE
#define GPIO_OP_PP   0   // output, push pull
#define GPIO_OP_OD   1   // output, open drain

// output speed :  @GPIO_SPEED
#define GPIO_SPEED_LOW    0
#define GPIO_SPEED_MEDIUM 1
#define GPIO_SPEED_FAST   2
#define GPIO_SPEED_HS     3   // high speed output

// gpio pin  pull-up / pull down  :    @GPIO_PUPD
#define GPIO_FLOAT  0   // no pull up, pull down
#define GPIO_PU     1
#define GPIO_PD     2



void GPIO_Init(GPIO_Handle_t *pPGIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);
void GPIO_ClkCtrl(GPIO_RegDef_t *pGPIOx, uint8_t En);  // En means enable

uint8_t  GPIO_ReadPin(GPIO_RegDef_t *pGPIOx,uint8_t PinNumber);
uint16_t GPIO_ReadPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WritePin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber, uint8_t value);
void GPIO_WritePort(GPIO_RegDef_t *pGPIOx ,uint16_t value);
void GPIO_Togglepin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber);

void GPIO_IRQConfig(uint8_t IRQ_num, uint8_t En);
void GPIO_IRQ_Priorityconfig(uint8_t IRQ_num, uint32_t IRQ_priority);
void GPIO_IRQHandling(uint8_t PinNumber);


#endif /* INC_F411_GPIO_DRIVER_H_ */
