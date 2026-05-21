/*
 * f411_i2c_driver.h
 *
 *  Created on: Nov 19, 2025
 *      Author: Rishikesh
 */

#ifndef INC_F411_I2C_DRIVER_H_
#define INC_F411_I2C_DRIVER_H_

#include "stm32f411.h"

typedef struct
{
 uint32_t I2C_SCLSpeed;
 uint8_t I2C_DeviceAddress ;
 uint8_t I2C_ACKControl ;
 uint16_t I2C_FMDutyCycle ;

}I2C_Config;  // structure which user fills


typedef struct
{
 I2C_Regdef_t *I2C_base_addr ;
 I2C_Config I2C_conf ;
 uint8_t *pTxBuffer;
 uint8_t *pRxBuffer;
 uint32_t TxLen;
 uint32_t RxLen;      // bytes left to be received
 uint8_t TxRxState;  // stores communication state
 uint8_t DevAddr;
 uint32_t RxSize;    // size of the received message
 uint8_t rep_start;

}I2C_Handle_t;

// @I2C_SCLSpeed
#define I2C_SCL_SPEED_SM   100000  // 100khz
#define I2C_SCL_SPEED_FM2K 200000
#define I2C_SCL_SPEED_FM4K 400000

//@I2C_ACKControl
#define I2C_ACK_ENABLE    1
#define I2C_ACK_DISABLE   0

//@I2C_FMDutyCycle
#define I2C_FM_DUTY2      0   // Tlow = 2*Thigh
#define I2C_FM_DUTY16BY9  1   // Tlow = 1.7*Thigh


#define I2C_READY       0
#define I2C_BUSY_IN_RX  1
#define I2C_BUSY_IN_TX  2


#define I2C_EV_TX_COMPLETE 0
#define I2C_EV_RX_COMPLETE 1
#define I2C_EV_STOP        2


#define I2C_ERROR_BERR     3
#define I2C_ERROR_ARLO     4
#define I2C_ERROR_AF       5
#define I2C_ERROR_OVR      6
#define I2C_ERROR_TIMEOUT  7
#define I2C_EV_DATA_REQ    8
#define I2C_EV_DATA_RCV    9



void I2C_ClkCtrl(I2C_Regdef_t *pI2Cx, uint8_t En) ;  // enables the I2C peripheral clock in RCC
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_Regdef_t *pI2Cx);

uint32_t RCC_GetPCLK1Value(void);

void I2C_IRQConfig(uint8_t IRQ_num, uint8_t En);
void I2C_IRQ_Priorityconfig(uint8_t IRQ_num, uint32_t IRQ_priority);

void I2C_PeripheralControl(I2C_Regdef_t *pI2Cx,uint8_t En); // enables the i2c peripheral from PE bit in CR1
uint8_t I2C_Get_flagstatus(I2C_Regdef_t *pI2Cx, uint32_t flagname);

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv);


void I2C_SendData(I2C_Handle_t *pI2C, uint8_t *pTxBuffer, uint32_t Len, uint8_t slaveaddr, uint8_t rep_start);
void I2C_ReceiveData(I2C_Handle_t *pI2C, uint8_t *pRxBuffer, uint32_t Len, uint8_t slaveaddr, uint8_t rep_start);

void I2C_SlaveSendData(I2C_Regdef_t *pI2C, uint8_t data_byte);
uint8_t I2C_SlaveReceiveData(I2C_Regdef_t *pI2C);

uint8_t I2C_SendDataIT(I2C_Handle_t *pI2C, uint8_t *pTxBuffer, uint32_t Len, uint8_t slaveaddr, uint8_t rep_start);
uint8_t I2C_ReceiveDataIT(I2C_Handle_t *pI2C, uint8_t *pRxBuffer, uint32_t Len, uint8_t slaveaddr, uint8_t rep_start);

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_close_transmission(I2C_Handle_t *pI2CHandle);

void I2C_SlaveEnableDisableCallback(I2C_Regdef_t *I2Cx,uint8_t en);

#endif /* INC_F411_I2C_DRIVER_H_ */
