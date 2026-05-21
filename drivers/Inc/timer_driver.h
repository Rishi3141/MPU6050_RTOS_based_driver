/*
 * timer_driver.h
 *
 *  Created on: Mar 26, 2026
 *      Author: Rishikesh
 */

#ifndef INC_TIMER_DRIVER_H_
#define INC_TIMER_DRIVER_H_


void IRQInterruptConfig(uint8_t IRQnum,uint8_t En);
void IRQPriorityConfig(uint8_t IRQnum,uint8_t IRQpriority);


#endif /* INC_TIMER_DRIVER_H_ */
