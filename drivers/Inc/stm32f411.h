/*
 * stm32f411.h
 *
 *  Created on: Aug 20, 2025
 *      Author: Rishikesh
 */
  // This header file describes your micro controller
#ifndef INC_STM32F411_H_
#define INC_STM32F411_H_

#include <stddef.h>
#include <stdint.h>

#define ENABLE  1
#define DISABLE 0
#define SET ENABLE
#define RESET DISABLE

#define __vo volatile

// ****************** ARM CORTEX M4 Specific Section ******************

#define NVIC_PRI0 0
#define NVIC_PRI1 1
#define NVIC_PRI2 2
#define NVIC_PRI3 3
#define NVIC_PRI4 4
#define NVIC_PRI5 5
#define NVIC_PRI6 6
#define NVIC_PRI7 7
#define NVIC_PRI8 8
#define NVIC_PRI9 9
#define NVIC_PRI10 10
#define NVIC_PRI11 11
#define NVIC_PRI12 12
#define NVIC_PRI13 13
#define NVIC_PRI14 14
#define NVIC_PRI15 15


#define NVIC_ISER0 ((__vo uint32_t*)0xE000E100)
#define NVIC_ISER1 ((__vo uint32_t*)0xE000E104)
#define NVIC_ISER2 ((__vo uint32_t*)0xE000E108)
#define NVIC_ISER3 ((__vo uint32_t*)0xE000E10C)

#define NVIC_ICER0 ((__vo uint32_t*)0xE000E180)
#define NVIC_ICER1 ((__vo uint32_t*)0xE000E184)
#define NVIC_ICER2 ((__vo uint32_t*)0xE000E188)
#define NVIC_ICER3 ((__vo uint32_t*)0xE000E18C)

#define INT_PRI_ADDR ((__vo uint32_t*)0xE000E400)  // Base address of interrupt priority register
#define NUM_PRI_BITS 4                          // number of priority bits actually implemented out of 8 bits
// *********************************************************************

#define IRQ_NO_EXTI0 		6
#define IRQ_NO_EXTI1 		7
#define IRQ_NO_EXTI2 		8
#define IRQ_NO_EXTI3 		9
#define IRQ_NO_EXTI4 		10
#define IRQ_NO_EXTI9_5 		23
#define IRQ_NO_EXTI15_10 	40
#define IRQ_NO_SPI1			35
#define IRQ_NO_SPI2         36
#define IRQ_NO_SPI3         51
#define IRQ_NO_SPI4         84
#define IRQ_NO_I2C1_EV      31
#define IRQ_NO_I2C1_ER      32
#define IRQ_NO_I2C2_EV      33
#define IRQ_NO_I2C2_ER      34
#define IRQ_NO_I2C3_EV      72
#define IRQ_NO_I2C3_ER      73
#define IRQ_NO_USART1	    37
#define IRQ_NO_USART2	    38
#define IRQ_NO_USART3	    39
#define IRQ_NO_UART4	    52
#define IRQ_NO_UART5	    53
#define IRQ_NO_USART6	    71



#define FLASH_BASEADDR  0x08000000U    // 'U' means unsigned
#define SRAM1_BASEADDR  0x20000000U
#define SRAM1_SIZE      0x00020000U   //  128 KB
#define ROM_ADDR        0x1FFF0000U   // System Memory
#define SRAM_ADDR       SRAM1_BASEADDR


#define PERI_BASEADDR      0x40000000U
#define APB1_PERI_BASEADDR PERI_BASEADDR
#define APB2_PERI_BASEADDR 0x40010000U      // Addresses for APB2 peripherals starts from here
#define AHB1_PERI_BASEADDR 0x40020000U
#define AHB2_PERI_BASEADDR 0x50000000U

// Base addresses of peripherals connected on AHB1 Bus
#define GPIOA_BASEADDR AHB1_PERI_BASEADDR
#define GPIOB_BASEADDR (AHB1_PERI_BASEADDR + 0x0400U)  // offset added to AHB1 address
#define GPIOC_BASEADDR (AHB1_PERI_BASEADDR + 0x0800U)
#define GPIOD_BASEADDR (AHB1_PERI_BASEADDR + 0x0C00U)
#define GPIOE_BASEADDR (AHB1_PERI_BASEADDR + 0x1000U)
#define GPIOH_BASEADDR (AHB1_PERI_BASEADDR + 0x1C00U)
#define RCC_BASEADDR   (AHB1_PERI_BASEADDR + 0x3800U)

// Base addresses of peripherals connected to APB1 Bus
#define TIM2_BASEADDR   (APB1_PERI_BASEADDR)
#define I2C1_BASEADDR   (APB1_PERI_BASEADDR + 0x5400U)
#define I2C2_BASEADDR   (APB1_PERI_BASEADDR + 0x5800U)
#define I2C3_BASEADDR   (APB1_PERI_BASEADDR + 0x5C00U)
#define SPI2_BASEADDR   (APB1_PERI_BASEADDR + 0x3800)   // removed 'U' for test
#define SPI3_BASEADDR   (APB1_PERI_BASEADDR + 0x3C00U)
#define USART2_BASEADDR (APB1_PERI_BASEADDR + 0x4400U)


// Base addresses of peripherals connected to APB2 Bus
#define EXTI_BASEADDR     (APB2_PERI_BASEADDR + 0x3C00U)
#define SPI1_BASEADDR     (APB2_PERI_BASEADDR + 0x3000U)
#define USART1_BASEADDR   (APB2_PERI_BASEADDR + 0x1000U)
#define USART6_BASEADDR   (APB2_PERI_BASEADDR + 0x1400U)
#define SYSCFG_BASEADDR   (APB2_PERI_BASEADDR + 0x3800U)


#define TIM2  ((TIM2to5_Regdef_t*) TIM2_BASEADDR)

#define GPIOA ((GPIO_RegDef_t*) GPIOA_BASEADDR)
#define GPIOB ((GPIO_RegDef_t*) GPIOB_BASEADDR)
#define GPIOC ((GPIO_RegDef_t*) GPIOC_BASEADDR)
#define GPIOD ((GPIO_RegDef_t*) GPIOD_BASEADDR)
#define GPIOE ((GPIO_RegDef_t*) GPIOE_BASEADDR)
#define GPIOH ((GPIO_RegDef_t*) GPIOH_BASEADDR)

#define SPI1 ((SPI_Regdef_t*) SPI1_BASEADDR)
#define SPI2 ((SPI_Regdef_t*) SPI2_BASEADDR)
#define SPI3 ((SPI_Regdef_t*) SPI3_BASEADDR)


#define SYSCFG ((SYSCFG_Regdef_t*) SYSCFG_BASEADDR )
#define RCC    ((RCC_RegDef_t*)RCC_BASEADDR)
#define EXTI   ((EXTI_RegDef_t*)EXTI_BASEADDR)

typedef struct
{
	__vo uint32_t MODER   ;
	__vo uint32_t OTYPER  ;
	__vo uint32_t OSPEEDR ;
	__vo uint32_t PUPDR   ;
	__vo uint32_t IDR     ;
	__vo uint32_t ODR     ;
	__vo uint32_t BSRR    ;
	__vo uint32_t LCKR    ;
	__vo uint32_t AFR[2]  ;  // AFR[0] = AFR low reg , AFR[1] = AFR high reg



}  GPIO_RegDef_t;


typedef struct
{
  __vo uint32_t IMR  ; // interrupt mask register
  __vo uint32_t EMR  ; // event mask register
  __vo uint32_t RTSR ; // Rising trigger selection register
  __vo uint32_t FTSR ; // Falling trigger selection reg.
  __vo uint32_t SWIER; // Software interrupt event reg.
  __vo uint32_t PR;    // Pending register

}EXTI_RegDef_t; // NOTE : For typedef structures, suffix '_t' is used


typedef struct
{
  __vo uint32_t MEMRMP	;
  __vo uint32_t PMC ;
  __vo uint32_t EXTICR[4];
  __vo uint32_t reserved[2];
  __vo uint32_t CMPCR ;
}SYSCFG_Regdef_t;


typedef struct
{
	__vo uint32_t CR1 ; // control register 1
	__vo uint32_t CR2 ; // control register 2
	__vo uint32_t SR  ; // Status register
	__vo uint32_t DR  ;  // Data register
	__vo uint32_t CRCPR  ;  // CRC polynomial register
	__vo uint32_t RXCRC  ;  // received CRC register
	__vo uint32_t TXCRC  ;  // transmit CRC register
	__vo uint32_t I2SCFGR  ;  // I2S configuration register
	__vo uint32_t I2SPR  ;  // I2S prescaler register

}SPI_Regdef_t;




typedef struct
{
	__vo uint32_t CR1  ;  // control reg1
	__vo uint32_t CR2  ;  // control reg2
	__vo uint32_t OAR1 ;  // own address register
	__vo uint32_t OAR2 ;
	__vo uint32_t DR   ;  // data register
    __vo uint32_t SR1  ;  // status register
    __vo uint32_t SR2  ;
    __vo uint32_t CCR  ;  // clock control register
    __vo uint32_t TRISE ;
    __vo uint32_t FLTR  ;
}I2C_Regdef_t;

#define I2C1 ((I2C_Regdef_t*)I2C1_BASEADDR)
#define I2C2 ((I2C_Regdef_t*)I2C2_BASEADDR)
#define I2C3 ((I2C_Regdef_t*)I2C3_BASEADDR)

typedef struct
{
	__vo uint32_t SR ; // status reg
	__vo uint32_t DR ; // Data reg
	__vo uint32_t BRR ; // Baud rate reg.
	__vo uint32_t CR1 ;  // control reg 1
	__vo uint32_t CR2 ;  // control reg 2
	__vo uint32_t CR3 ;  // control reg 3
	__vo uint32_t GTPR; // Guard time and prescaler reg.


}USART_Regdef_t;

#define USART1 ((USART_Regdef_t*)USART1_BASEADDR)
#define USART2 ((USART_Regdef_t*)USART2_BASEADDR)
#define USART6 ((USART_Regdef_t*)USART6_BASEADDR)




typedef struct
{
  __vo uint32_t CR ;           // control reg
  __vo uint32_t PLLCFGR ;      // pll config reg
  __vo uint32_t CLCFGR ;       // clock config reg
  __vo uint32_t CIR ;          // clock interrupt reg
  __vo uint32_t AHB1RST ;     // AHB1  peripheral reset register
  __vo uint32_t AHB2RST ;     // AHB2 peripheral reset register
       uint32_t RESERVED1[2] ;
  __vo uint32_t APB1RST ;     // APB1 peripheral reset register
  __vo uint32_t APB2RST ;     // APB2 peripheral reset register
       uint32_t RESERVED2[2] ;
__vo  uint32_t AHB1ENR ;     // AHB1 peripheral clock enable register
__vo  uint32_t AHB2ENR ;     // AHB2 peripheral clock enable register
      uint32_t RESERVED3[2] ;
__vo  uint32_t APB1ENR ;     // AHB1 peripheral clock enable register
__vo  uint32_t APB2ENR ;     // AHB2 peripheral clock enable register
      uint32_t RESERVED4[2] ;
__vo  uint32_t AHB1LPENR ;     // AHB1 peripheral low power clock enable register
__vo  uint32_t AHB2LPENR ;     // AHB2 peripheral low power clock enable register
      uint32_t RESERVED5[2] ;
__vo  uint32_t APB1LPENR ;     // APB1 peripheral low power clock enable register
__vo  uint32_t APB2LPENR ;     // APB2 peripheral low power clock enable register
      uint32_t RESERVED6[2] ;
__vo  uint32_t BDCR ;     // Backup domain control register
__vo  uint32_t CSR ;     // Control and status register
      uint32_t RESERVED7[2] ;
__vo  uint32_t SSCGR ;     // Spread Spectrum clock generation register
__vo  uint32_t PLLI2S_CFGR ;     // PLLI2S configuration register
__vo  uint32_t DCK_CFGR ;     // Dedicated clock configuration register
}RCC_RegDef_t;

//RCC_RegDef_t *RCC = (RCC_RegDef_t * ) RCC_BASEADDR ;


// CLOCK ENABLE MACROS
#define GPIOA_CLK_EN()  (RCC->AHB1ENR |= 1 << 0 )
#define GPIOB_CLK_EN()  (RCC->AHB1ENR |= 1 << 1 )
#define GPIOC_CLK_EN()  (RCC->AHB1ENR |= 1 << 2 )
#define GPIOD_CLK_EN()  (RCC->AHB1ENR |= 1 << 3 )
#define GPIOE_CLK_EN()  (RCC->AHB1ENR |= 1 << 4 )
#define GPIOH_CLK_EN()  (RCC->AHB1ENR |= 1 << 7 )

#define TIM2_CLK_EN() 	(RCC->APB1ENR |= 1 << 0)

#define SPI1_CLK_EN() (RCC->APB2ENR |= 1 << 12)
#define SPI2_CLK_EN() (RCC->APB1ENR |= 1 << 14)
#define SPI3_CLK_EN() (RCC->APB1ENR |= 1 << 15)


#define I2C1_CLK_EN() (RCC->APB1ENR |= 1 << 21)
#define I2C2_CLK_EN() (RCC->APB1ENR |= 1 << 22)
#define I2C3_CLK_EN() (RCC->APB1ENR |= 1 << 23)

#define SYSCFG_CLK_EN() (RCC->APB2ENR |= 1 << 14)
#define SPI1_CLK_EN()   (RCC->APB2ENR |= 1 << 12)
#define USART1_CLK_EN() (RCC->APB2ENR |= 1 << 4)
#define USART2_CLK_EN() (RCC->APB1ENR |= 1 << 17)
#define USART6_CLK_EN() (RCC->APB2ENR |= 1 << 5)


// CLOCK DISABLE MACROS
#define GPIOA_CLK_DS() (RCC->AHB1ENR &= (~(1<<0)) )
#define GPIOB_CLK_DS() (RCC->AHB1ENR &= (~(1<<1)) )
#define GPIOC_CLK_DS() (RCC->AHB1ENR &= (~(1<<2)) )
#define GPIOD_CLK_DS() (RCC->AHB1ENR &= (~(1<<3)) )
#define GPIOE_CLK_DS() (RCC->AHB1ENR &= (~(1<<4)) )
#define GPIOH_CLK_DS() (RCC->AHB1ENR &= (~(1<<7)) )

#define SPI1_CLK_DS() (RCC->APB2ENR &= (~(1<<12)) )
#define SPI2_CLK_DS() (RCC->APB1ENR &= (~(1<<14)) )
#define SPI3_CLK_DS() (RCC->APB1ENR &= (~(1<<15)) )


#define I2C1_CLK_DS() (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_CLK_DS() (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_CLK_DS() (RCC->APB1ENR &= ~(1 << 23))

#define SYSCFG_CLK_DS() (RCC->APB2ENR &= ~(1 << 14))
#define USART1_CLK_DS() (RCC->APB2ENR &= ~(1 << 4) )
#define USART2_CLK_DS() (RCC->APB1ENR &= ~(1 << 17))
#define USART6_CLK_DS() (RCC->APB2ENR &= ~(1 << 5) )

#define GPIOA_REG_RESET()  do{RCC->AHB1RST |= 1 << 0 ; RCC->AHB1RST &= ~(1 << 0);  } while(0)
#define GPIOB_REG_RESET()  do{RCC->AHB1RST |= 1 << 1 ; RCC->AHB1RST &= ~(1 << 1);  } while(0)
#define GPIOC_REG_RESET()  do{RCC->AHB1RST |= 1 << 2 ; RCC->AHB1RST &= ~(1 << 2);  } while(0)
#define GPIOD_REG_RESET()  do{RCC->AHB1RST |= 1 << 3 ; RCC->AHB1RST &= ~(1 << 3);  } while(0)
#define GPIOE_REG_RESET()  do{RCC->AHB1RST |= 1 << 4 ; RCC->AHB1RST &= ~(1 << 4);  } while(0)
#define GPIOH_REG_RESET()  do{RCC->AHB1RST |= 1 << 7 ; RCC->AHB1RST &= ~(1 << 7);  } while(0)

#define SPI1_REG_RESET() do{ RCC->APB2RST |= 1 << 12 ; RCC->AHB1RST &= ~(1 << 12);  } while(0)
#define SPI2_REG_RESET() do{ RCC->APB1RST |= 1 << 14 ; RCC-> AHB1RST &= ~(1 << 14); } while(0)
#define SPI3_REG_RESET() do{ RCC->APB1RST |= 1 << 15 ; RCC-> AHB1RST &= ~(1 << 15); } while(0)

#define I2C1_REG_RESET() do{ RCC->APB1RST |= 1 << 21 ; RCC->AHB1RST &= ~(1 << 21);  } while(0)
#define I2C2_REG_RESET() do{ RCC->APB1RST |= 1 << 22 ; RCC-> AHB1RST &= ~(1 << 22); } while(0)
#define I2C3_REG_RESET() do{ RCC->APB1RST |= 1 << 23 ; RCC-> AHB1RST &= ~(1 << 23); } while(0)



// Bit position definitions for SPI peripheral registers

#define SPI_CR1_CPHA       0
#define SPI_CR1_CPOL       1
#define SPI_CR1_MSTR       2   // master-slave selection bit
#define SPI_CR1_BR         3   // Baud rate bits
#define SPI_CR1_SPE        6
#define SPI_CR1_LSBFIRST   7
#define SPI_CR1_SSI        8
#define SPI_CR1_SSM        9
#define SPI_CR1_RXONLY     10
#define SPI_CR1_DFF        11
#define SPI_CR1_CRCNEXT    12
#define SPI_CR1_CRCEN      13
#define SPI_CR1_BIDIOE     14
#define SPI_CR1_BIDIMODE   15


/*
 * Bit position definitions SPI_CR2
 */
#define SPI_CR2_RXDMAEN		 			0
#define SPI_CR2_TXDMAEN				 	1
#define SPI_CR2_SSOE				 	2
#define SPI_CR2_FRF						4
#define SPI_CR2_ERRIE					5
#define SPI_CR2_RXNEIE				 	6
#define SPI_CR2_TXEIE					7


/*
 * Bit position definitions SPI_SR
 */
#define SPI_SR_RXNE						0
#define SPI_SR_TXE				 		1
#define SPI_SR_CHSIDE				 	2
#define SPI_SR_UDR					 	3
#define SPI_SR_CRCERR				 	4
#define SPI_SR_MODF					 	5
#define SPI_SR_OVR					 	6
#define SPI_SR_BSY					 	7
#define SPI_SR_FRE					 	8


// Bit position for I2C_CR1
#define I2C_CR1_PE        0
#define I2C_CR1_SMBUS     1
#define I2C_CR1_SMBTYPE   3
#define I2C_CR1_ENARP     4
#define I2C_CR1_ENPEC     5
#define I2C_CR1_ENGC      6
#define I2C_CR1_NOSTRETCH 7
#define I2C_CR1_START     8
#define I2C_CR1_STOP      9
#define I2C_CR1_ACK      10
#define I2C_CR1_POS      11
#define I2C_CR1_PEC      12
#define I2C_CR1_ALERT    13
#define I2C_CR1_SWRST    15

// Bit position for I2C_CR2
#define I2C_CR2_ITERREN      8
#define I2C_CR2_ITEVTEN      9
#define I2C_CR2_ITBUFEN      10
#define I2C_CR2_DMAEN        11
#define I2C_CR2_LAST         12


// Bit position for I2C_SR1
#define I2C_SR1_SB          0 // start bit
#define I2C_SR1_ADDR        1  // address sent
#define I2C_SR1_BTF         2  // byte transfer finished
#define I2C_SR1_ADD10       3  // 10 bit header sent
#define I2C_SR1_STOPF       4  // stop detection
#define I2C_SR1_RxNE        6  // data register not empty
#define I2C_SR1_TxE         7  // data register empty
#define I2C_SR1_BERR        8  // Bus error
#define I2C_SR1_ARLO        9
#define I2C_SR1_AF          10
#define I2C_SR1_OVR         11
#define I2C_SR1_PECERR      12
#define I2C_SR1_TIMEOUT     14
#define I2C_SR1_SMBALERT    15

// Bit position for I2C_SR2
#define I2C_SR2_MSL          0   // master/slave mode
#define I2C_SR2_BUSY         1   //  busy
#define I2C_SR2_TRA          2   // transmitter/receiver
#define I2C_SR2_GENCALL      4
#define I2C_SR2_SMBDEFAULT   5
#define I2C_SR2_SMBHOST      6
#define I2C_SR2_DUALF        7




// Bit position for I2C_CCR
#define I2C_DUTY 14
#define I2C_FS   15

//Bit positions for USART SR
#define USART_SR_PE   0     // parity error
#define USART_SR_FE   1     // framing error
#define USART_SR_NF   2    // noise detected flag
#define USART_SR_ORE  3    // Overrun error
#define USART_SR_IDLE 4    // IDLE line detected
#define USART_SR_RXNE 5    // read data register not empty
#define USART_SR_TC   6    // Transmission complete
#define USART_SR_TXE  7   // Transmit data register empty
#define USART_SR_LBD  8   // LIN break etection flag
#define USART_SR_CTS  9   //CTS flag



//Bit positions for USART CR1
#define USART_CR_SBK     0     // Send break
#define USART_CR_RWU     1     // Receiver wakeup
#define USART_CR_RE      2    // Receiver enable
#define USART_CR_TE      3    // Transmitter enable
#define USART_CR_IDLEIE  4    // IDLE interrupt enable
#define USART_CR_RXNEIE  5    // RXNE interrupt enable
#define USART_CR_TCIE    6    // Transmission complete interrupt enable
#define USART_CR_TXEIE   7   // TXE interrupt enable
#define USART_CR_PEIE    8   // PE interrupt enable
#define USART_CR_PS      9   // Parity Selection
#define USART_CR_PCE    10    // Parity Control enable
#define USART_CR_WAKE   11    // Wakeup method
#define USART_CR_M      12    // Word Length
#define USART_CR_UE     13   // USART enable
#define USART_CR_OVER8  15   // oversampling mode




//Bit positions for USART CR2

#define USART_CR2_LBDL     5    // LIN break detection length
#define USART_CR2_LBDIE    6    // LIN break detection interrupt enable
#define USART_CR2_LBCL     8   // last bit clock pulse
#define USART_CR2_CPHA     9   // clock phase
#define USART_CR2_CPOL    10   // clock polarity
#define USART_CR2_CLKEN   11   // clock enable
#define USART_CR2_STOP    12   // STOP bits (bit 12 and 13)
#define USART_CR2_LINEN   14   // LIN mode enable


//Bit positions for USART CR3

#define USART_CR3_EIE     0    // Error interrupt enable
#define USART_CR3_IREN    1    // IrDA mode enable
#define USART_CR3_IRLP    2   // IrDA low power
#define USART_CR3_HDSEL   3   // Half duplex selection
#define USART_CR3_NACK    4   // Smartcard NACK enable
#define USART_CR3_SCEN    5   // Smartcard mode enable
#define USART_CR3_DMAR    6   // DMA enable receiver
#define USART_CR3_DMAT    7   // DMA enable transmitter
#define USART_CR3_RTSE    8   // RTS enable
#define USART_CR3_CTSE    9   // CTS enable
#define USART_CR3_CTSIE  10   // CTS interrupt enable
#define USART_CR3_ONEBIT 11   // One sample bit method enable




#include "f411_gpio_driver.h"
#include "timer_driver.h"
#include "clock_setup.h"
#include "f411_i2c_driver.h"
//#include "f411_rcc_driver.h"

#endif /* INC_STM32F411_H_ */
