/*
 * MPU6050.h
 *
 *  Created on: Jan 5, 2026
 *      Author: Rishikesh
 */

#ifndef INC_MPU6050_H_
#define INC_MPU6050_H_

#include "stm32f411.h"

#define device_address_0 0x68   // When external A0 = 0
#define device_address_1 0x69   // When external A1 pin is 1

#define MPU_SPEED_100KHZ 100000
#define MPU_SPEED_400KHZ 400000


// arguments for mpu6050_clock_select function
#define INTERNAL_CLOCK  0x00
#define EXT_32KHZ_CLOCK 0x04
#define EXT_19MHZ_CLOCK 0x05
#define STOP_CLOCK      0x07


// low power wakeup frequency (for mpu6050_low_power_enable function)
#define WAKE_UP_1HZ 0x00 // 1.25 Hz to be specific
#define WAKE_UP_5HZ 0x01
#define WAKE_UP_20HZ 0x02
#define WAKE_UP_40HZ 0x03

//Gyroscope Full scale values in degrees/second (for GYRO_Config function)
#define GYRO_FS_250  0
#define GYRO_FS_500  1
#define GYRO_FS_1000 2
#define GYRO_FS_2000 3


//Accelerometer Full scale values in g(for  ACCEL_Config function)
#define ACCEL_FS_2  0
#define ACCEL_FS_4  1
#define ACCEL_FS_8  2
#define ACCEL_FS_16 3

// DLPF Bandwidth values (for DLPF_Config function)
#define DLPF_BW_260 0 // These BW values are for Accelerometer
#define DLPF_BW_184 1 // For Gyroscope they slightly vary
#define DLPF_BW_94  2
#define DLPF_BW_44  3
#define DLPF_BW_21  4
#define DLPF_BW_10  5
#define DLPF_BW_5   6




// MPU internal registers
#define SMPRT_DIV    0x19  // Sample Rate Divider
#define CONFIG       0x1A  // DLPF config
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define INT_ENABLE   0x38  // Interrupt Enable
#define INT_STATUS   0x3A  // Interrupt Status (Check this register when interrupt

#define ACCEL_XOUT_H 0x3B  // Accelerometer Measurements
#define ACCEL_XOUT_L 0x3C
#define ACCEL_YOUT_H 0x3D
#define ACCEL_YOUT_L 0x3E
#define ACCEL_ZOUT_H 0x3F
#define ACCEL_ZOUT_L 0x40

#define TEMP_OUT_H 0x41    // Temp. measurements
#define TEMP_OUT_L 0x42  // temp. in Degrees = (TEMP_OUT reg value (signed))/340 + 36.53

#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44
#define GYRO_YOUT_H 0x45
#define GYRO_YOUT_L 0x46
#define GYRO_ZOUT_H 0x47
#define GYRO_ZOUT_L 0x48


#define PWR_MGMT_1   0x6B  // Power management 1
#define PWR_MGMT_2   0x6C





typedef struct
{
 I2C_Regdef_t* which_I2C ; // possible values : I2C1 , I2C2
 uint32_t I2C_speed ;  // possible values : MPU_SPEED_100KHZ , MPU_SPEED_400KHZ
 uint8_t dev_addr  ;  // default is 0x68

 int16_t accelX_val ;
 int16_t accelY_val ;
 int16_t accelZ_val ;
 int16_t gyroX_val ;
 int16_t gyroY_val ;
 int16_t gyroZ_val ;
 int16_t temp_val ;

}mpu6050_Regdef;

void mpu6050_init(mpu6050_Regdef* mpu) ;
void mpu6050_DeInit(mpu6050_Regdef* mpu);

void mpu6050_reset(mpu6050_Regdef* mpu);
void mpu6050_sleep(mpu6050_Regdef* mpu, uint8_t state);
void mpu6050_cycle_mode(mpu6050_Regdef* mpu, uint8_t state);
void mpu6050_temp_disable(mpu6050_Regdef* mpu, uint8_t state);
void mpu6050_clock_select(mpu6050_Regdef* mpu, uint8_t state);
void mpu6050_low_power_enable(mpu6050_Regdef* mpu, uint8_t state,uint8_t LP_wake_freq);


void SMPRT_div_set(mpu6050_Regdef* mpu,uint8_t divisor);
void DLPF_Config(mpu6050_Regdef* mpu, uint8_t state);
void ACCEL_Config(mpu6050_Regdef* mpu, uint8_t state);
void GYRO_Config(mpu6050_Regdef* mpu, uint8_t state);
void ACCEL_RECEIVE(mpu6050_Regdef* mpu, uint8_t axis);
void GYRO_RECEIVE(mpu6050_Regdef* mpu, uint8_t state);

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle,uint8_t AppEv);



//void mpu6050_power_manage(mpu6050_Regdef* mpu, uint8_t state) ;
//void mpu6050_temp_disable(mpu6050_Regdef* mpu, uint8_t state) ;
//void mpu6050_clock_select(mpu6050_Regdef* mpu, uint8_t state) ;



#endif /* INC_MPU6050_H_ */
