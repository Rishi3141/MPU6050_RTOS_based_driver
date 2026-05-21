/*
 * MPU6050.c
 *
 *  Created on: Jan 5, 2026
 *      Author: Rishikesh
 */


#include "MPU6050.h"
//#include "f411_i2c_driver.h"
#include <stdint.h>

I2C_Handle_t I2C_instant ;
GPIO_Handle_t GPIO_instant ;

uint8_t data[2];

void delay_2(void);
void I2C_bus_recover(mpu6050_Regdef* mpu);

void delay_2(void) // 187ms  required : 0.01ms
	{
		for(uint32_t i = 0 ; i < (13/2) ; i++)
		{

		}
	}


void I2C_bus_recover(mpu6050_Regdef* mpu)
{
	 if(mpu->which_I2C == I2C1)
	  {

		  GPIO_instant.pGPIO_Base_Addr = GPIOB ;
		  GPIO_instant.pinConfig.GPIO_PinNumber = GPIO_PIN_6 ; // SCL PB6
		  GPIO_instant.pinConfig.GPIO_PinMode = GPIO_MODE_OUT ;
	   // GPIO_instant.pinConfig.GPIO_PinAltFunMode = 4 ;
		  GPIO_instant.pinConfig.GPIO_PinOPType = GPIO_OP_OD  ;
		  GPIO_instant.pinConfig.GPIO_PinPuPdControl = GPIO_PU ;
		  GPIO_instant.pinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST ;
	//	  GPIO_DeInit(GPIO_instant.pGPIO_Base_Addr);
		  GPIO_ClkCtrl(GPIO_instant.pGPIO_Base_Addr, ENABLE);
		  GPIO_WritePin(GPIOB ,GPIO_PIN_6,ENABLE);
		  GPIO_Init(&GPIO_instant) ;

		  GPIO_instant.pinConfig.GPIO_PinNumber = GPIO_PIN_7 ; // SDA PB7 and PB9(better)
		  GPIO_WritePin(GPIOB ,GPIO_PIN_7,ENABLE);
		  GPIO_Init(&GPIO_instant) ;
	  //  GPIO_WritePin(GPIOB ,GPIO_PIN_7,ENABLE); // SDA = 1 before toggling SCL

		  for(uint32_t i = 0 ; i < 9 ; i++)
		  {
		    GPIO_WritePin(GPIOB ,GPIO_PIN_6,DISABLE);
			delay_2();
			GPIO_WritePin(GPIOB ,GPIO_PIN_6,ENABLE);
			delay_2();

		  }

	  }


     else if(mpu->which_I2C == I2C2)
     {


     }



}

void mpu6050_init(mpu6050_Regdef* mpu) // initializes I2C and GPIO pins required for it.
{
      I2C_PeripheralControl(mpu->which_I2C,DISABLE);

      //bus recovery
      I2C_bus_recover(mpu);


	 // I2C1_REG_RESET();

      if(mpu->which_I2C == I2C1)
      {

    	  // GPIOB configuration for I2C1
    	  GPIO_instant.pGPIO_Base_Addr = GPIOB ;
    	  GPIO_DeInit(GPIO_instant.pGPIO_Base_Addr);
    	  GPIO_instant.pinConfig.GPIO_PinNumber = GPIO_PIN_6 ; // SCL PB6
    	  GPIO_instant.pinConfig.GPIO_PinMode = GPIO_MODE_ALTFN ;
    	  GPIO_instant.pinConfig.GPIO_PinAltFunMode = 4 ;
    	  GPIO_instant.pinConfig.GPIO_PinOPType = GPIO_OP_OD  ;
    	  GPIO_instant.pinConfig.GPIO_PinPuPdControl = GPIO_PU ;
    	  GPIO_instant.pinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST ;
    	  GPIO_Init(&GPIO_instant) ;

    	//  GPIO_instant.pGPIO_Base_Addr = GPIOB ;
    	  GPIO_instant.pinConfig.GPIO_PinNumber = GPIO_PIN_7 ; // SDA PB7 and PB9(better)
    	  GPIO_Init(&GPIO_instant) ;
    	//  GPIO_instant.pinConfig.GPIO_PinMode = GPIO_MODE_ALTFN ;
    	//  GPIO_instant.pinConfig.GPIO_PinAltFunMode = 4 ;
    	//  GPIO_instant.pinConfig.GPIO_PinOPType = GPIO_OP_OD ;
    	//  GPIO_instant.pinConfig.GPIO_PinPuPdControl = GPIO_PU ;
    	//  GPIO_instant.pinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST ;


      }

      else if(mpu->which_I2C == I2C2)
      {
    	  // GPIOB configuration for I2C2
    	  GPIO_instant.pGPIO_Base_Addr = GPIOB ;
    	  GPIO_instant.pinConfig.GPIO_PinNumber = GPIO_PIN_10 ; // SCL  need to veryfy this pin
    	  GPIO_instant.pinConfig.GPIO_PinMode = GPIO_MODE_ALTFN ;
    	  GPIO_instant.pinConfig.GPIO_PinAltFunMode = 4 ;
    	  GPIO_instant.pinConfig.GPIO_PinOPType = GPIO_OP_OD  ;
    	  GPIO_instant.pinConfig.GPIO_PinPuPdControl = GPIO_FLOAT ;
    	  GPIO_instant.pinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST ;
    	  GPIO_Init(&GPIO_instant) ;

    	  GPIO_instant.pGPIO_Base_Addr = GPIOB ;
    	  GPIO_instant.pinConfig.GPIO_PinNumber = GPIO_PIN_11 ; // SDA need to verify this pin
    	  GPIO_instant.pinConfig.GPIO_PinMode = GPIO_MODE_ALTFN ;
    	  GPIO_instant.pinConfig.GPIO_PinAltFunMode = 4 ;
    	  GPIO_instant.pinConfig.GPIO_PinOPType = GPIO_OP_OD ;
    	  GPIO_instant.pinConfig.GPIO_PinPuPdControl = GPIO_FLOAT ;
    	  GPIO_instant.pinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST ;
    	  GPIO_Init(&GPIO_instant) ;
      }


	  I2C_instant.I2C_base_addr = mpu->which_I2C ;
      I2C_instant.I2C_conf.I2C_DeviceAddress = mpu->dev_addr ;
      I2C_instant.I2C_conf.I2C_SCLSpeed = mpu->I2C_speed ;
      I2C_instant.I2C_conf.I2C_ACKControl = I2C_ACK_ENABLE  ; // hardcoded
      I2C_instant.I2C_conf.I2C_FMDutyCycle =  I2C_FM_DUTY2  ; // hardcoded
      I2C_instant.TxRxState = I2C_READY  ;



      I2C_Init(&I2C_instant);
      I2C_PeripheralControl(mpu->which_I2C,ENABLE);
	  if(mpu->which_I2C == I2C1)
	  {
       I2C_IRQConfig(IRQ_NO_I2C1_EV, ENABLE) ;
       I2C_IRQConfig(IRQ_NO_I2C1_ER, ENABLE) ;
	  }
	  else if(mpu->which_I2C == I2C2)
	  {
       I2C_IRQConfig(IRQ_NO_I2C2_EV, ENABLE) ;
       I2C_IRQConfig(IRQ_NO_I2C2_ER, ENABLE) ;
	  }
	 mpu6050_reset(mpu);
     mpu6050_sleep(mpu,DISABLE);  // Wake up from sleep after initialization
     mpu6050_cycle_mode(mpu, DISABLE);
     mpu6050_temp_disable(mpu,DISABLE); // Enable temp. sensor
     mpu6050_clock_select(mpu,INTERNAL_CLOCK );
}



void mpu6050_DeInit(mpu6050_Regdef* mpu)
{

	mpu6050_reset(mpu);
    I2C_PeripheralControl(mpu->which_I2C,DISABLE);
    I2C_DeInit(mpu->which_I2C);
	if(mpu->which_I2C == I2C1)
	{
		GPIO_DeInit(GPIOB);
	}
	else if(mpu->which_I2C == I2C2)
	{
		GPIO_DeInit(GPIOB); // assuming pins are in GPIOB for I2C2
	}

	else if(mpu->which_I2C == I2C3)
	{
		GPIO_DeInit(GPIOC); // assuming pins are in GPIOC for I2C3
	}


}


//POWER MANAGEMENT FUNCTIONS

void mpu6050_sleep(mpu6050_Regdef* mpu, uint8_t state)
{
	while(I2C_instant.TxRxState != I2C_READY);
	data[0] = PWR_MGMT_1 ;
//	data[1] = 0 ;
	while(I2C_SendDataIT(&I2C_instant, data, 1, mpu->dev_addr, 1) != I2C_READY ); // Set address as PWR_MGMT_1
	while(I2C_ReceiveDataIT(&I2C_instant,(data + 1),1,mpu->dev_addr,0) != I2C_READY);
	while(I2C_instant.TxRxState != I2C_READY); // makes sure that data[1] is written first
	if(state == 1) // sleep mode
	{

		data[1] |= 0x40 ; // setting sleep bit
	}
	else // wake up
	{

	   data[1] &= ~0x40 ; // clearing sleep bit

	}


	while(I2C_SendDataIT(&I2C_instant, data, 2, mpu->dev_addr, 0) != I2C_READY );
}



void mpu6050_reset(mpu6050_Regdef* mpu)
{
	while(I2C_instant.TxRxState != I2C_READY);
	data[0] = PWR_MGMT_1 ;
	data[1] = 0x80 ; // setting reset bit
    //Reset Bit automatically clears, PWR_MGMT_1 reg. is set to 0x40 (sleep mode) on reset


	while(I2C_SendDataIT(&I2C_instant, data, 2, mpu->dev_addr, 0) != I2C_READY);
	while(I2C_instant.TxRxState != I2C_READY); // make sure communication is complete before deactivating peripherals (when using mpu6050_DeInit)
}

void mpu6050_cycle_mode(mpu6050_Regdef* mpu, uint8_t state)
{
	while(I2C_instant.TxRxState != I2C_READY);
	data[0] = PWR_MGMT_1 ;
//	data[1] = 0 ;
	while(I2C_SendDataIT(&I2C_instant, data, 1, mpu->dev_addr, 1) != I2C_READY ); // Set register pointer to PWR_MGMT_1
	while(I2C_ReceiveDataIT(&I2C_instant,(data + 1),1,mpu->dev_addr,0) != I2C_READY);
	while(I2C_instant.TxRxState != I2C_READY); // makes sure that data[1] is written first


	//I2C_SendDataIT(&I2C_instant, data, 1, mpu->dev_addr, 0);
	if(state == 1) // enable cycle mode
	{
		data[1] |= 0x20 ; //
	}
	else //
	{
		data[1] &= ~0x20 ; //
	}
	// send data over I2C

	while(I2C_SendDataIT(&I2C_instant, data, 2, mpu->dev_addr, 0) != I2C_READY);
}

void mpu6050_temp_disable(mpu6050_Regdef* mpu, uint8_t state)
{
	while(I2C_instant.TxRxState != I2C_READY);
	data[0] = PWR_MGMT_1 ;
//	data[1] = 0 ;
	while(I2C_SendDataIT(&I2C_instant, data, 1, mpu->dev_addr, 1) != I2C_READY ); // Set ptr address as PWR_MGMT_1
	while(I2C_ReceiveDataIT(&I2C_instant,(data + 1),1,mpu->dev_addr,0) != I2C_READY);
	while(I2C_instant.TxRxState != I2C_READY); // makes sure that data[1] is written first

	if(state == 1) // disable temperature sensor
	{
		data[1] |= 0x08 ; //
	}
	else // enable
	{
		data[1] &= ~0x08 ; //
	}
	// send data over I2C

	while(I2C_SendDataIT(&I2C_instant, data, 2, mpu->dev_addr, 0) != I2C_READY);
}

void mpu6050_clock_select(mpu6050_Regdef* mpu, uint8_t state)
{
	while(I2C_instant.TxRxState != I2C_READY);
	data[0] = PWR_MGMT_1 ;
	//data[1] = 0 ;
	while(I2C_SendDataIT(&I2C_instant, data, 1, mpu->dev_addr, 1) != I2C_READY ); // Set ptr address as PWR_MGMT_1
	while(I2C_ReceiveDataIT(&I2C_instant,(data + 1),1,mpu->dev_addr,0) != I2C_READY);
	while(I2C_instant.TxRxState != I2C_READY); // makes sure that data[1] is written first

	data[1] &= ~(0x07);
	data[1] |= state ;



	// send data over I2C

	while(I2C_SendDataIT(&I2C_instant, data, 2, mpu->dev_addr, 0) != I2C_READY);
}



void mpu6050_low_power_enable(mpu6050_Regdef* mpu, uint8_t state, uint8_t LP_wake_freq)
{

	while(I2C_instant.TxRxState != I2C_READY);
     data[0] = PWR_MGMT_2 ;
	// data[1] = 0 ;
	while(I2C_SendDataIT(&I2C_instant, data, 1, mpu->dev_addr, 1) != I2C_READY ); // Set ptr address as PWR_MGMT_2
	 while(I2C_ReceiveDataIT(&I2C_instant,(data + 1),1,mpu->dev_addr,0) != I2C_READY);
	 while(I2C_instant.TxRxState != I2C_READY); // makes sure that data[1] is written first

	if(state == 1) // enable low power mode
	{
		mpu6050_cycle_mode(mpu,ENABLE);
	    mpu6050_sleep(mpu, DISABLE);
	    mpu6050_temp_disable(mpu,ENABLE);
	    data[1] &= ~(0xC0);
	    data[1] |= (LP_wake_freq << 6);
		data[1] |= 0x07 ; // All Gyro axis on standby
	}
	else // disable low power mode
	{
		mpu6050_cycle_mode(mpu,DISABLE);
		mpu6050_temp_disable(mpu,DISABLE);
		data[1] &= ~0x07 ; // All Gyro axis enabled

	}
	// send data over I2C

	while(I2C_SendDataIT(&I2C_instant, data, 2, mpu->dev_addr, 0) != I2C_READY);
}



void SMPRT_div_set(mpu6050_Regdef* mpu,uint8_t divisor)
{
	while(I2C_instant.TxRxState != I2C_READY);
	data[0] = SMPRT_DIV ;
	data[1] = divisor ;

	// sample_rate = Gyro_output_rate/(1 + divisor)
	// Gyro_output_rate = 8Khz when DLPF_CFG = 0 or 7
	//                  = 1Khz otherwise


	while(I2C_SendDataIT(&I2C_instant, data, 2, mpu->dev_addr, 0) != I2C_READY);

}


void DLPF_Config(mpu6050_Regdef* mpu, uint8_t state)
{
	while(I2C_instant.TxRxState != I2C_READY);
	data[0] = CONFIG ;
	//data[1] = 0 ;
	while(I2C_SendDataIT(&I2C_instant, data, 1, mpu->dev_addr, 1) != I2C_READY ); // Set ptr address as PWR_MGMT_1
	while(I2C_ReceiveDataIT(&I2C_instant,(data + 1),1,mpu->dev_addr,0) != I2C_READY);
	while(I2C_instant.TxRxState != I2C_READY); // makes sure that data[1] is written first

	data[1] &= ~(0x07);
	data[1] |= state ;



	// send data over I2C

	while(I2C_SendDataIT(&I2C_instant, data, 2, mpu->dev_addr, 0) != I2C_READY);

}

void ACCEL_Config(mpu6050_Regdef* mpu, uint8_t state)
{
	while(I2C_instant.TxRxState != I2C_READY);
	data[0] = CONFIG ;
	//data[1] = 0 ;
	while(I2C_SendDataIT(&I2C_instant, data, 1, mpu->dev_addr, 1) != I2C_READY );
	while(I2C_ReceiveDataIT(&I2C_instant,(data + 1),1,mpu->dev_addr,0) != I2C_READY);
	while(I2C_instant.TxRxState != I2C_READY); // makes sure that data[1] is written first

	data[1] &= ~( 0x03 << 3); // Clear AFS_SEL bit fields
	data[1] |=  (state << 3) ;



	// send data over I2C

	while(I2C_SendDataIT(&I2C_instant, data, 2, mpu->dev_addr, 0) != I2C_READY);

}

void GYRO_Config(mpu6050_Regdef* mpu, uint8_t state)
{
	while(I2C_instant.TxRxState != I2C_READY);
	data[0] = CONFIG ;
	//data[1] = 0 ;
	while(I2C_SendDataIT(&I2C_instant, data, 1, mpu->dev_addr, 1) != I2C_READY );
	while(I2C_ReceiveDataIT(&I2C_instant,(data + 1),1,mpu->dev_addr,0) != I2C_READY);
	while(I2C_instant.TxRxState != I2C_READY); // makes sure that data[1] is written first

	data[1] &= ~( 0x03 << 3); // Clear AFS_SEL bit fields
	data[1] |=  (state << 3) ;



	// send data over I2C

	while(I2C_SendDataIT(&I2C_instant, data, 2, mpu->dev_addr, 0) != I2C_READY);

}

void ACCEL_RECEIVE(mpu6050_Regdef* mpu, uint8_t axis)
{
	uint8_t temp_val[6];
	while(I2C_instant.TxRxState != I2C_READY);
	uint8_t reg_addr = ACCEL_XOUT_H ;

	while(I2C_SendDataIT(&I2C_instant,&reg_addr, 1, mpu->dev_addr, 1) != I2C_READY ); // Set ptr address as ACCEL_XOUT_H
	while(I2C_ReceiveDataIT(&I2C_instant,temp_val,6,mpu->dev_addr,0) != I2C_READY);
	while(I2C_instant.TxRxState != I2C_READY); // makes sure that data[1] is written first

	mpu->accelX_val = ((uint16_t)temp_val[0] << 8 ) | ((uint16_t)temp_val[1]);
	mpu->accelY_val = ((uint16_t)temp_val[2] << 8 ) | ((uint16_t)temp_val[3]);
	mpu->accelZ_val = ((uint16_t)temp_val[4] << 8 ) | ((uint16_t)temp_val[5]);


}



void GYRO_RECEIVE(mpu6050_Regdef* mpu, uint8_t state)
{
	    uint8_t temp_val[6];
		while(I2C_instant.TxRxState != I2C_READY);
		uint8_t reg_addr = GYRO_XOUT_H ;//0x43

		while(I2C_SendDataIT(&I2C_instant,&reg_addr, 1, mpu->dev_addr, 1) != I2C_READY ); // Set ptr address as ACCEL_XOUT_H
		while(I2C_ReceiveDataIT(&I2C_instant,temp_val,6,mpu->dev_addr,0) != I2C_READY);
		while(I2C_instant.TxRxState != I2C_READY); // makes sure that data[1] is written first

		mpu->gyroX_val = ((uint16_t)temp_val[0] << 8 ) | ((uint16_t)temp_val[1]);
		mpu->gyroY_val = ((uint16_t)temp_val[2] << 8 ) | ((uint16_t)temp_val[3]);
		mpu->gyroZ_val = ((uint16_t)temp_val[4] << 8 ) | ((uint16_t)temp_val[5]);

}

// Single write sequence  :  Start + Slave Address + Register Address + Data + Stop
// Multiple write sequence:  Start + Slave Address + Register Address + Data1 + Data2 + .... + DataN + Stop (Register address auto increments)

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv)
{

}


void I2C1_EV_IRQHandler(void)  // IRQ handlers for I2C1 and I2C2 , check names properly
{
 I2C_EV_IRQHandling(&I2C_instant) ;
}
void I2C2_EV_IRQHandler(void)
{
 I2C_EV_IRQHandling(&I2C_instant) ;
}

void I2C1_ER_IRQHandler(void)
{

}


