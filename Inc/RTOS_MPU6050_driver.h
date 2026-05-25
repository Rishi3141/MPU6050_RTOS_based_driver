/*
 * RTOS_MPU6050_driver.h
 *
 *  Created on: May 21, 2026
 *      Author: Rishikesh
 */

#ifndef RTOS_MPU6050_DRIVER_H_
#define RTOS_MPU6050_DRIVER_H_


#define ACQ_10ms 10
#define TICK_TIME_SEC 0.01
#define ACCEL_RES 16384.0
#define GYRO_RES 131.0


// Thresholds for task_act, user should set according to their requirement
#define ACC_X_TRIG 1.2
#define ACC_Y_TRIG 1.2
#define ACC_Z_TRIG 1.2
#define VEL_X_TRIG 2.0
#define VEL_Y_TRIG 2.0
#define VEL_Z_TRIG 2.0
#define ANG_VEL_X_TRIG 150.0
#define ANG_VEL_Y_TRIG 150.0
#define ANG_VEL_Z_TRIG 150.0
#define ANG_X_TRIG 90.0
#define ANG_Y_TRIG 90.0
#define ANG_Z_TRIG 90.0





typedef struct
{
 __vo int16_t accel_X;
 __vo int16_t accel_Y;
 __vo int16_t accel_Z;
 __vo int16_t gyro_X;
 __vo int16_t gyro_Y;
 __vo int16_t gyro_Z;
 __vo uint16_t temp ;


}mpu_rawdata_t;



typedef struct
{
	float accel_ax;
	float accel_ay;
	float accel_az;
	float vel_ax ;
	float vel_ay ;
	float vel_az ;
	float degrees_sec_gx  ;
	float degrees_sec_gy  ;
	float degrees_sec_gz ;
	float degrees_gx  ;
	float degrees_gy ;
	float degrees_gz  ;
	float temp_degrees ;
}mpu_processed_data;  // This struct will store final calculations which user can use

mpu_processed_data global_processed_data ;  // Use this global variable to access final calculated readings


typedef struct
{
	 int64_t ax ;
     int64_t ay ;
	 int64_t az ;
	 int64_t gx ;
	 int64_t gy ;
	 int64_t gz ;

}accumulated_values;

typedef struct
{
	__vo int mean_ax ;
	__vo int mean_ay ;
	__vo int mean_az ;
	__vo int mean_gx ;
	__vo int mean_gy ;
	__vo int mean_gz ;
	__vo int var_ax ;
	__vo int var_ay ;
	__vo int var_az ;
	__vo int var_gx ;
	__vo int var_gy ;
	__vo int var_gz ;
	__vo int thresh_ax ;  // treshold = 3* Standard deviation
	__vo int thresh_ay ;
	__vo int thresh_az ;
	__vo int thresh_gx ;
	__vo int thresh_gy ;
	__vo int thresh_gz ;



}mpu_calibration_stats_t;

mpu_calibration_stats_t mpu_calib1 ;



#endif /* RTOS_MPU6050_DRIVER_H_ */
