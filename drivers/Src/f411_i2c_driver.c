/*
 * f411_i2c_driver.c
 *
 *  Created on: Nov 20, 2025
 *      Author: Rishikesh
 */
#include "f411_i2c_driver.h"

uint16_t AHB_PreScaler[8] = {2,4,8,16,64,128,256,512};
uint8_t APB1_PreScaler[4] = {2,4,8,16};


static void Clear_addr_flag(I2C_Handle_t *pI2CHandle);
static void  I2C_Master_TXEinterrupt_Handler(I2C_Handle_t *pI2CHandle);
static void  I2C_Master_RXNEinterrupt_Handler(I2C_Handle_t *pI2CHandle);


static void Clear_addr_flag(I2C_Handle_t *pI2CHandle)
{
	uint32_t dummy_read;


	if(pI2CHandle->I2C_base_addr->SR2 & (1 << I2C_SR2_MSL))
	{
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize ==  1)
			{
				pI2CHandle->I2C_base_addr->CR1 &= ~(1 << 10) ; // Disable acking
				   dummy_read = pI2CHandle->I2C_base_addr->SR1 ;
				   dummy_read = pI2CHandle->I2C_base_addr->SR2 ;  //clear ADDR bit, (read SR1 followed by reading SR2)
				   (void)dummy_read;
			}



		}
		else
		{
			   dummy_read = pI2CHandle->I2C_base_addr->SR1 ;
			   dummy_read = pI2CHandle->I2C_base_addr->SR2 ;  //clear ADDR bit, (read SR1 followed by reading SR2)
			   (void)dummy_read;
		}

	 }

	else  // device is in slave mode
	{
	   dummy_read = pI2CHandle->I2C_base_addr->SR1 ;
	   dummy_read = pI2CHandle->I2C_base_addr->SR2 ;  //clear ADDR bit, (read SR1 followed by reading SR2)
	   (void)dummy_read;

	}
}

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	pI2CHandle->I2C_base_addr->CR2 &= ~(1 << I2C_CR2_ITBUFEN); // clear interrupt control bits
	pI2CHandle->I2C_base_addr->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL ;
	pI2CHandle->RxLen  = 0;
	pI2CHandle->RxSize = 0;

	pI2CHandle->I2C_base_addr->CR1 |= (1 << 10) ; // Enable Acking again
}

void I2C_close_transmission(I2C_Handle_t *pI2CHandle)
{
	pI2CHandle->I2C_base_addr->CR2 &= ~(1 << I2C_CR2_ITBUFEN); // clear interrupt control bits
    pI2CHandle->I2C_base_addr->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

    pI2CHandle->TxRxState = I2C_READY;
    pI2CHandle->pTxBuffer = NULL ;
    pI2CHandle->TxLen = 0 ;
}



void I2C_ClkCtrl(I2C_Regdef_t *pI2Cx, uint8_t En)  // En means enable
{
	 if (En == ENABLE)
	   {
		  if(pI2Cx == I2C1)
		  {
			  I2C1_CLK_EN();
		  }
		  else if(pI2Cx == I2C2)
		  {
			  I2C2_CLK_EN();
		  }
		  else // (pSPIx == SPI3)
		  {
			  I2C3_CLK_EN();
		  }


	   }

	   else
	   {
		   if(pI2Cx == I2C1)
		   		  {
		   			  I2C1_CLK_DS();
		   		  }
		   		  else if(pI2Cx == I2C2)
		   		  {
		   			  I2C2_CLK_DS();
		   		  }
		   		  else // (pSPIx == SPI3)
		   		  {
		   			  I2C3_CLK_DS();
		   		  }



	   }


}

void I2C_SlaveSendData(I2C_Regdef_t *pI2C, uint8_t data_byte)
{
	pI2C->DR = data_byte ;
}
uint8_t I2C_SlaveReceiveData(I2C_Regdef_t *pI2C)
{
	return (uint8_t)pI2C->DR ;
}


void I2C_SendData(I2C_Handle_t *pI2C, uint8_t *pTxBuffer, uint32_t Len, uint8_t slaveaddr , uint8_t rep_start)
{

	pI2C->I2C_base_addr->CR1 |= (1 << 8); // Generate start condition

	while(!I2C_Get_flagstatus(pI2C->I2C_base_addr,I2C_SR1_SB) );  // wait till start condition is generated

	// send address to slave with r/w bit
    slaveaddr = slaveaddr << 1 ;
    slaveaddr &= ~(1);
    pI2C->I2C_base_addr->DR = slaveaddr ;

    while(!I2C_Get_flagstatus(pI2C->I2C_base_addr,I2C_SR1_ADDR) ); // wait till ADDR bit is set in SR1
 //  volatile uint32_t dummy_read ;
 //   dummy_read = pI2C->I2C_base_addr->SR1 ;
 //   dummy_read = pI2C->I2C_base_addr->SR2 ;  //clear ADDR bit, (read SR1 followed by reading SR2)
 //   (void)dummy_read; // to get rid of 'unused variable' warning
    Clear_addr_flag(pI2C);

    while(Len > 0)
    {
    	while(! I2C_Get_flagstatus(pI2C->I2C_base_addr,I2C_SR1_TxE));
    	pI2C->I2C_base_addr->DR = *pTxBuffer;
    	pTxBuffer++;
    	Len-- ;
    }

    while(! I2C_Get_flagstatus(pI2C->I2C_base_addr,I2C_SR1_TxE));
    while(! I2C_Get_flagstatus(pI2C->I2C_base_addr,I2C_SR1_BTF));

    if(rep_start == 0)
    pI2C->I2C_base_addr->CR1 |= (1 << 9); // Generate stop condition



}

// function to send data as a master
uint8_t I2C_SendDataIT(I2C_Handle_t *pI2C, uint8_t *pTxBuffer, uint32_t Len, uint8_t slaveaddr, uint8_t rep_start)
{
	uint8_t busystate = pI2C->TxRxState ;

		if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
		{
			pI2C->pTxBuffer = pTxBuffer ;
			pI2C->TxLen = Len;
			pI2C->TxRxState = I2C_BUSY_IN_TX;
		//	busystate = I2C_BUSY_IN_TX ; //Newly added on 18 Jan
			pI2C->DevAddr = slaveaddr;
			pI2C->rep_start = rep_start;

			//Implement code to Generate START Condition
			pI2C->I2C_base_addr->CR1 |= (1 << 8);



			pI2C->I2C_base_addr->CR2 |= ( 1 << I2C_CR2_ITBUFEN); // enable ITBUFEN Control Bit
			pI2C->I2C_base_addr->CR2 |= ( 1 << I2C_CR2_ITEVTEN); // enable ITEVFEN Control Bit
			pI2C->I2C_base_addr->CR2 |= ( 1 << I2C_CR2_ITERREN); // enable ITERREN Control Bit

		}

		return busystate;

}



void I2C_ReceiveData(I2C_Handle_t *pI2C, uint8_t *pRxBuffer, uint32_t Len, uint8_t slaveaddr ,uint8_t rep_start)
{
	 // STEP1 : Generate start condition
	pI2C->I2C_base_addr->CR1 |= (1 << 8);
	while(!I2C_Get_flagstatus(pI2C->I2C_base_addr,I2C_SR1_SB) );  // wait till start condition is generated

	// STEP2 : send address to slave with r/w bit
	slaveaddr = slaveaddr << 1 ;
	slaveaddr |= 1;
	pI2C->I2C_base_addr->DR = slaveaddr ;

	// STEP3 :  wait till ADDR bit is set in SR1
    while(!I2C_Get_flagstatus(pI2C->I2C_base_addr,I2C_SR1_ADDR) );

    if(Len == 1)
    {
    	pI2C->I2C_base_addr->CR1 &= ~(1 << 10) ; // Disable acking



 //   	volatile uint32_t dummy_read ;
 //		dummy_read = pI2C->I2C_base_addr->SR1 ;
//		dummy_read = pI2C->I2C_base_addr->SR2 ;  //clear ADDR bit, (read SR1 followed by reading SR2)
//		(void)dummy_read; // to get rid of 'unused variable' warning
    	Clear_addr_flag(pI2C);

		 while(!I2C_Get_flagstatus(pI2C->I2C_base_addr,I2C_SR1_RxNE) );  // wait till RxNE flag is set

		 if(rep_start == 0)
    	 pI2C->I2C_base_addr->CR1 |= (1 << 9); // Generate stop condition


		 *pRxBuffer = pI2C->I2C_base_addr->DR ;  // read DR

    }

    else if (Len > 1 )
    {
    //	volatile uint32_t dummy_read ;
	//	dummy_read = pI2C->I2C_base_addr->SR1 ;
	//	dummy_read = pI2C->I2C_base_addr->SR2 ;  //clear ADDR bit, (read SR1 followed by reading SR2)
		Clear_addr_flag(pI2C);
	//	(void)dummy_read; // to get rid of 'unused variable' warning


		for(uint32_t i = Len ; i > 0 ; i--)
		{
			 while(!I2C_Get_flagstatus(pI2C->I2C_base_addr,I2C_SR1_RxNE) );  // wait till RxNE flag is set


			if(i == 2)
			{
		    	pI2C->I2C_base_addr->CR1 &= ~(1 << 10) ; // Disable acking

		    	if(rep_start == 0)
		        pI2C->I2C_base_addr->CR1 |= (1 << 9); // Generate stop condition


			}

			 *pRxBuffer = pI2C->I2C_base_addr->DR ;

			 pRxBuffer++ ;


		}


    }


  if(pI2C->I2C_conf.I2C_ACKControl == I2C_ACK_ENABLE ) // enable Acking before leaving this function
    pI2C->I2C_base_addr->CR1 |= (1 << 10) ;

}






uint8_t I2C_ReceiveDataIT(I2C_Handle_t *pI2C, uint8_t *pRxBuffer, uint32_t Len, uint8_t slaveaddr, uint8_t rep_start)
{
	uint8_t busystate = pI2C->TxRxState;

		if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
		{
			pI2C->pRxBuffer = pRxBuffer;
			pI2C->RxLen = Len;
			pI2C->TxRxState = I2C_BUSY_IN_RX ;
		//	busystate = I2C_BUSY_IN_RX ; //Newly added on 18 Jan
			pI2C->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception
			pI2C->DevAddr = slaveaddr;
			pI2C->rep_start = rep_start;


			pI2C->I2C_base_addr->CR1 |= (1 << I2C_CR1_START); // Generate start condition
		    pI2C->I2C_base_addr->CR2 |= (1 << I2C_CR2_ITBUFEN);	//enable buffer interrupts
		    pI2C->I2C_base_addr->CR2 |= (1 << I2C_CR2_ITEVTEN );//enable events interrupts
		    pI2C->I2C_base_addr->CR2 |= (1 << I2C_CR2_ITERREN );//enable error interrupts

		}

		return busystate;

}

void  I2C_Master_TXEinterrupt_Handler(I2C_Handle_t *pI2CHandle)
{
	//

}

void  I2C_Master_RXNEinterrupt_Handler(I2C_Handle_t *pI2CHandle)
{

	  if(pI2CHandle->RxSize == 1)
	  {
		  *pI2CHandle->pRxBuffer = pI2CHandle->I2C_base_addr->DR;
		  pI2CHandle->RxLen--;
	  }

	  else if(pI2CHandle->RxSize > 1)
	  {
			if(pI2CHandle->RxLen == 2)
			{
				pI2CHandle->I2C_base_addr->CR1 &= ~(1 << 10) ; // Disable acking

			}

			*pI2CHandle->pRxBuffer = pI2CHandle->I2C_base_addr->DR;
			  pI2CHandle->pRxBuffer++;
			  pI2CHandle->RxLen--;
	  }

	  if(pI2CHandle->RxLen == 0)
	  {
		  if(pI2CHandle->rep_start == 0) // if repeated start is disabled
		  pI2CHandle->I2C_base_addr->CR1 |= (1 << 9); // Generate stop condition

		  I2C_CloseReceiveData(pI2CHandle);

		  I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_COMPLETE);

	  }
}



void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{

	//Interrupt handling for both master and slave mode of a device

	uint32_t temp1,temp2, temp3 ;

	temp1 = pI2CHandle->I2C_base_addr->CR2 & (1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->I2C_base_addr->CR2 & (1 << I2C_CR2_ITBUFEN);
	temp3 = pI2CHandle->I2C_base_addr->SR1 & (1 << I2C_SR1_SB); // Start bit is set in SendDataIT and ReceiveDataIT functions

	//1. Handle For interrupt generated by SB event
	//	Note : SB flag is only applicable in Master mode
     if(temp1 && temp3)  // SB flag = 1  ->  start condition generated. execute address phase next
     {



    	 if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX) // if transmitting, set 8th bit to 0
    	 {
    		 uint8_t slaveaddr ;
    		 slaveaddr = pI2CHandle->DevAddr;
    		 slaveaddr = slaveaddr << 1 ;
    		 slaveaddr &= ~(1);
    		 pI2CHandle->I2C_base_addr->DR = slaveaddr ;
    	 }

    	 else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
    	 {
    		 uint8_t slaveaddr ;
			 slaveaddr = pI2CHandle->DevAddr;
			 slaveaddr = slaveaddr << 1 ;
			 slaveaddr |=  1;
			 pI2CHandle->I2C_base_addr->DR = slaveaddr ;

    	 }


     }

	//2. Handle For interrupt generated by ADDR event
	//Note : When master mode : Address is sent
	//		 When Slave mode   : Address matched with own address
 	temp3 = pI2CHandle->I2C_base_addr->SR1 & (1 << I2C_SR1_ADDR);
 	if(temp1 && temp3)
 	{
 		Clear_addr_flag(pI2CHandle);

 	}


	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event
 	temp3 = pI2CHandle->I2C_base_addr->SR1 & (1 << I2C_SR1_BTF);
	if(temp1 && temp3)  // if BTF = 1 , generate stop condition next
	{
	  if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
	  {
			if(pI2CHandle->I2C_base_addr->SR1 & (1 << I2C_SR1_TxE))
			{
				if(pI2CHandle->TxLen == 0)
				{
					// generate stop condition
					if(pI2CHandle->rep_start == 0) // if repeated start is disabled
					pI2CHandle->I2C_base_addr->CR1 |= (1 << 9); // Generate stop condition

					I2C_close_transmission(pI2CHandle);

					I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_COMPLETE);

				}
			}
	  }

	  else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
	  {
		  // do nothing
	  }


	}

	//4. Handle For interrupt generated by STOPF event
	// Note : Stop detection flag is applicable only slave mode . For master this flag will never be set
	temp3 = pI2CHandle->I2C_base_addr->SR1 & (1 << I2C_SR1_STOPF);
	if(temp1 && temp3)
	{
		//clear stop flag (read SR1 and then write to CR1)
		pI2CHandle->I2C_base_addr->CR1 |= 0x0000 ;
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);

	}

	//5. Handle For interrupt generated by TXE event
	temp3 = pI2CHandle->I2C_base_addr->SR1 & (1 << I2C_SR1_TxE);

	if(temp1 && temp2 && temp3 )
		{
			 if(pI2CHandle->I2C_base_addr->SR2 & (1 << I2C_SR2_MSL))
			 {
			   if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
			   {
				  if(pI2CHandle->TxLen > 0)  //I2C_Master_TXEinterrupt_Handler
				  {
					  uint8_t dat;
					  dat = *(pI2CHandle->pTxBuffer);
					  pI2CHandle->I2C_base_addr->DR = dat;
					  pI2CHandle->TxLen-- ;
					  pI2CHandle->pTxBuffer++ ;
				  }

			   }
			 }

			 else
			 {
				 if(pI2CHandle->I2C_base_addr->SR2 & (1 << I2C_SR2_TRA)) // ensure that device is in transmitter mode
				 {
				   I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_DATA_REQ);

			      }

		}
	}



	//6. Handle For interrupt generated by RXNE event
	temp3 = pI2CHandle->I2C_base_addr->SR1 & (1 << I2C_SR1_RxNE);

		if(temp1 && temp2 && temp3  )
		 {
		   if(pI2CHandle->I2C_base_addr->SR2  & (1 << I2C_SR2_MSL) ) // check if device is in master mode
		   {
				  if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
				  {

					  I2C_Master_RXNEinterrupt_Handler(pI2CHandle);
				  }

			 //	  else
			 //	  {
             //	Clear_addr_flag(pI2CHandle);
            //	 }
		   }

		   else
		   {
			   if(!(pI2CHandle->I2C_base_addr->SR2 & (1 << I2C_SR2_TRA))) // ensure that device is in receiver mode
				 {
				   I2C_ApplicationEventCallback(pI2CHandle,I2C_EV_DATA_RCV);

				  }
		   }

		 }





}

void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	uint32_t temp1,temp2;

	    //Know the status of  ITERREN control bit in the CR2
		temp2 = (pI2CHandle->I2C_base_addr->CR2) & ( 1 << I2C_CR2_ITERREN);


	/***********************Check for Bus error************************************/
		temp1 = (pI2CHandle->I2C_base_addr->SR1) & ( 1<< I2C_SR1_BERR);
		if(temp1  && temp2 )
		{
			//This is Bus error

			//Implement the code to clear the buss error flag
			pI2CHandle->I2C_base_addr->SR1 &= ~( 1 << I2C_SR1_BERR);

			//Implement the code to notify the application about the error
		   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
		}

	/***********************Check for arbitration lost error************************************/
		temp1 = (pI2CHandle->I2C_base_addr->SR1) & ( 1 << I2C_SR1_ARLO );
		if(temp1  && temp2)
		{
			//This is arbitration lost error

			//Implement the code to clear the arbitration lost error flag
			pI2CHandle->I2C_base_addr->SR1 &= ~( 1 << I2C_SR1_ARLO);
			//Implement the code to notify the application about the error
			 I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_ARLO);

		}

	/***********************Check for ACK failure  error************************************/

		temp1 = (pI2CHandle->I2C_base_addr->SR1) & ( 1 << I2C_SR1_AF);
		if(temp1  && temp2)
		{
			//This is ACK failure error

		    //Implement the code to clear the ACK failure error flag
			pI2CHandle->I2C_base_addr->SR1 &= ~(1 << I2C_SR1_AF) ;

			//Implement the code to notify the application about the error
			I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_AF);
		}

	/***********************Check for Overrun/underrun error************************************/
		temp1 = (pI2CHandle->I2C_base_addr->SR1) & ( 1 << I2C_SR1_OVR);
		if(temp1  && temp2)
		{
			//This is Overrun/underrun

		    //Implement the code to clear the Overrun/underrun error flag
			pI2CHandle->I2C_base_addr->SR1 &= ~(1 << I2C_SR1_OVR) ;
			//Implement the code to notify the application about the error
			I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_OVR);
		}

	/***********************Check for Time out error************************************/
		temp1 = (pI2CHandle->I2C_base_addr->SR1) & ( 1 << I2C_SR1_TIMEOUT);
		if(temp1  && temp2)
		{
			//This is Time out error

		    //Implement the code to clear the Time out error flag
			pI2CHandle->I2C_base_addr->SR1 &= ~(1 << I2C_SR1_TIMEOUT) ;
			//Implement the code to notify the application about the error
			I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_TIMEOUT);
		}




}




void I2C_Init(I2C_Handle_t *pI2CHandle)
{
  uint32_t tempreg = 0 ;


  I2C_ClkCtrl(pI2CHandle->I2C_base_addr , ENABLE);
  pI2CHandle->I2C_base_addr->CR1 = 0 ;

 // tempreg |= ((pI2CHandle->I2C_conf.I2C_ACKControl) << 10);
 // pI2CHandle->I2C_base_addr->CR1 |= tempreg ;    //   PROB1 solved

  // Configure the FREQ field in CR2
  tempreg = 0 ;
  tempreg |=  RCC_GetPCLK1Value()/1000000U ;
  pI2CHandle->I2C_base_addr->CR2 = (tempreg & 0x3F);

  tempreg = 0 ;
  tempreg |= pI2CHandle->I2C_conf.I2C_DeviceAddress << 1 ;
  tempreg |= (1 << 14); // 14th bit should always be kept 1
  pI2CHandle->I2C_base_addr->OAR1 = tempreg ;

  // Configure the CCR field in CCR register
  uint16_t ccr_value = 0 ;
  tempreg = 0 ;
  if(pI2CHandle->I2C_conf.I2C_SCLSpeed <=  I2C_SCL_SPEED_SM) // standard mode
  {
	  ccr_value =  RCC_GetPCLK1Value()/(2*pI2CHandle->I2C_conf.I2C_SCLSpeed);
	  tempreg |= ccr_value & 0xFFF ;
  }
  else
  {
     tempreg |= (1 << 15);   // set mode bit in CCR to fast mode
     tempreg |= (pI2CHandle->I2C_conf.I2C_FMDutyCycle << 14);
     if(pI2CHandle->I2C_conf.I2C_FMDutyCycle == I2C_FM_DUTY2)
     {
   	  ccr_value =  RCC_GetPCLK1Value()/(3*pI2CHandle->I2C_conf.I2C_SCLSpeed);

     }
     else
     {
      ccr_value =  RCC_GetPCLK1Value()/(25*pI2CHandle->I2C_conf.I2C_SCLSpeed);

     }
     tempreg |= (ccr_value & 0xFFF);
  }

  pI2CHandle->I2C_base_addr->CCR = tempreg ;


 // TRISE register configuration
  if(pI2CHandle->I2C_conf.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
  {

	  tempreg = (RCC_GetPCLK1Value()/1000000U) + 1 ;
  }
  else
  {
	  tempreg = ( (RCC_GetPCLK1Value()*300)/1000000U) + 1 ;
  }

  pI2CHandle->I2C_base_addr->TRISE = tempreg & 0x3F ;


}

uint32_t RCC_GetPLLOutputclock(void)
{
	uint32_t result = 0;

	// not implemented yet
	return result ;
}


uint32_t RCC_GetPCLK1Value(void)  // function to get APB1 clock frequency
{
  uint32_t pclk1 , Systemclk ;
  uint8_t clksrc,temp,ahbp,apb1 ;  // to hold bit position 2 and 3 from RCC_CFGR


  clksrc = ((RCC->CLCFGR >> 2) & 0x3)   ;
  if(clksrc == 0)  // internal clock
  {
	  Systemclk = 16000000;
  }
  else if(clksrc == 1) // external clock (8Mhz on discovery board)
  {
	  Systemclk = 8000000;
  }
  else if(clksrc == 2) // external clock (8Mhz on discovery board)
   {
 	  Systemclk = RCC_GetPLLOutputclock();
   }


  temp = (RCC->CLCFGR >> 4) & 0xF ;
  if(temp < 8)
  {
	  ahbp = 1 ;
  }
  else
  {
	  ahbp = AHB_PreScaler[temp-8];


  }


  temp = (RCC->CLCFGR >> 10) & 0x7 ;
  if(temp < 4)
  {
	  apb1 = 1 ;
  }
  else
  {
	  apb1 = APB1_PreScaler[temp-4];


  }

  pclk1 = (Systemclk/ahbp)/apb1 ;

  return pclk1 ;


}

void I2C_DeInit(I2C_Regdef_t *pI2Cx)
{
	 if(pI2Cx == I2C1)
	  {
		  I2C1_CLK_DS();
	  }
	  else if(pI2Cx == I2C2)
	  {
		  I2C2_CLK_DS();
	  }
	  else // (pSPIx == SPI3)
	  {
		  I2C3_CLK_DS();
	  }



}

uint8_t I2C_Get_flagstatus(I2C_Regdef_t *pI2Cx, uint32_t flagname) // checks status of various flags in SPI status register
{
	if(((pI2Cx->SR1) & (1 << flagname)))
	{
		return 1 ;
	}
	else
		return 0 ;

}


void I2C_IRQConfig(uint8_t IRQ_num, uint8_t En)
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

void I2C_PeripheralControl(I2C_Regdef_t *pI2Cx,uint8_t En)
{
	 I2C_ClkCtrl(pI2Cx , ENABLE);
	if(En == ENABLE)
	{
		pI2Cx->CR1 |= 1 << I2C_CR1_PE  ;

	}
	else
	{
		pI2Cx->CR1 &= ~(1 << I2C_CR1_PE);
	}
}

void I2C_SlaveEnableDisableCallback(I2C_Regdef_t *I2Cx,uint8_t en)
{
	if(en == ENABLE)
	{
		I2Cx->CR2 |= (1 << I2C_CR2_ITEVTEN);
		I2Cx->CR2 |= (1 << I2C_CR2_ITBUFEN);
		I2Cx->CR2 |= (1 << I2C_CR2_ITERREN);

	}
	else
	{
		I2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);
		I2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);
		I2Cx->CR2 &= ~(1 << I2C_CR2_ITERREN);

	}
}





void I2C_IRQ_Priorityconfig(uint8_t IRQ_num, uint32_t IRQ_priority)
{

	  uint8_t ipr_reg_num = IRQ_num/4 ; // gives register number from irq number
	  uint8_t pos         = IRQ_num%4 ; // gives byte position (0 to 3)
	  *(INT_PRI_ADDR + ipr_reg_num) |= (IRQ_priority << (8*pos + (8 - NUM_PRI_BITS)) );
}

// __attribute__((weak)) void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv)
//{
	// weak implementation. The user application may override this function


//}
