
#include <stdint.h>
#include "stm32f411.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include <math.h>

//#include "portable.h"

#include "MPU6050.h"



mpu6050_Regdef mpu1 ;
QueueHandle_t rawdata_queue ;
SemaphoreHandle_t bin_semph1 ;
SemaphoreHandle_t bin_calib_semph2;
int sqr(int16_t num);
int mod(int num);

float calculate_accel(int16_t, int);



typedef struct
{
 __vo int16_t accel_X;
 __vo int16_t accel_Y;
 __vo int16_t accel_Z;
 __vo int16_t gyro_X;
 __vo int16_t gyro_Y;
 __vo int16_t gyro_Z;
 __vo int16_t temp ;


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
}mpu_processed_data;

mpu_processed_data global_processed_data ;

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



void process_data(mpu_rawdata_t *data ,mpu_calibration_stats_t *stats, uint8_t level );
//void TIM2_IRQHandler(void);
void control_led_init(void);
void start_TIM2(void);

void task_mpu_acquire(void *params);
void task_process(void *params);
void task_act(void *params);

void task_act(void *params)
{

	while(1)
	{




		        if(mod(global_processed_data.degrees_gz) > 90 )
		      		        {
		      		        	GPIO_WritePin(GPIOD, 13,SET);

		      		        }

		      		        else
		      		        	GPIO_WritePin(GPIOD, 13,RESET);





	}

}



void task_mpu_acquire(void *params)
{
	TickType_t lastWakeTime; // Code outside infinite loop runs ONLY ONCE
    lastWakeTime = xTaskGetTickCount(); // fetch the tick count at the instance this function is called.
    mpu_rawdata_t mpu_data_acq ;

    while(1)
    {
     //   GPIO_Togglepin(GPIOD, 12);

        ACCEL_RECEIVE(&mpu1, 1);
        GYRO_RECEIVE(&mpu1, 1);

        mpu_data_acq.accel_X = mpu1.accelX_val ;
        mpu_data_acq.accel_Y = mpu1.accelY_val ;
        mpu_data_acq.accel_Z = mpu1.accelZ_val ;
        mpu_data_acq.gyro_X = mpu1.gyroX_val ;
        mpu_data_acq.gyro_Y = mpu1.gyroY_val ;
        mpu_data_acq.gyro_Z = mpu1.gyroZ_val ;


        xQueueSend(rawdata_queue, &mpu_data_acq , portMAX_DELAY);


        // create one struct. that can store all the data and send it over queue


        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10));  // Run a task every 10ms
       // vTaskDelay : Blocks the task for specified number of ticks
      // pdMS_TO_TICKS : converts milliseconds to ticks
    }
}

void task_process(void *params)
{
//	TickType_t lastWakeTime; // Code outside infinite loop runs ONLY ONCE
//	lastWakeTime = xTaskGetTickCount(); // fetch the tick count at the instance this function is called.
    mpu_rawdata_t mpu_data_pr ;
    mpu_processed_data final_data ;
    int accel_val = 0 ;
    int gyro_val = 0 ;
    int count = 0 ;
    float degrees = 0 ;

    xSemaphoreTake(bin_calib_semph2,portMAX_DELAY); // Start the processing task after calibration data is available

    while(1)
    {


        xQueueReceive(rawdata_queue,&mpu_data_pr, portMAX_DELAY);

        process_data(&mpu_data_pr, &mpu_calib1,4); // only process gyro readings


  //      vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(10));
     //   vTaskDelay(pdMS_TO_TICKS(1000));  // 1 sec
    }
}




void task_calibrate(void* params)
{
  int count = 0 ;
  const int initial_period = 20 ;
  const int mean_period = 1000 ;
  const int var_period = 2000 ;
  const int max_count = var_period ;
  uint8_t treshold_factor = 3 ;

  GPIO_Togglepin(GPIOD, 15);
  mpu_rawdata_t mpu_data_cal ;
  accumulated_values acc_struct1 ;
  acc_struct1.ax = 0 ;
  acc_struct1.ay = 0;
  acc_struct1.az = 0 ;
  acc_struct1.gx = 0 ;
  acc_struct1.gy = 0;
  acc_struct1.gz = 0 ;


  global_processed_data.accel_ax = 0 ;
  global_processed_data.accel_ay = 0 ;
  global_processed_data.accel_az = 0 ;
  global_processed_data.vel_ax = 0 ;
  global_processed_data.vel_ay = 0 ;
  global_processed_data.vel_az = 0 ;
  global_processed_data.degrees_gx = 0 ;
  global_processed_data.degrees_gy = 0 ;
  global_processed_data.degrees_gz = 0 ;
  global_processed_data.degrees_sec_gx = 0 ;
  global_processed_data.degrees_sec_gy = 0 ;
  global_processed_data.degrees_sec_gy = 0 ;




	while(1)
	{

        xQueueReceive(rawdata_queue,&mpu_data_cal, portMAX_DELAY);

        if (count >= initial_period && count <= mean_period) // ignore initial 20 readings (200 ms)
        {
           acc_struct1.ax += mpu_data_cal.accel_X ;
           acc_struct1.ay += mpu_data_cal.accel_Y ;
           acc_struct1.az += mpu_data_cal.accel_Z ;
           acc_struct1.gx += mpu_data_cal.gyro_X ;
           acc_struct1.gy += mpu_data_cal.gyro_Y ;
           acc_struct1.gz += mpu_data_cal.gyro_Z ;


        }


        if (count == mean_period)
        {
        	int mean_denominator ;
        	mean_denominator = mean_period - initial_period + 1 ;
        	// Calculate mean first
        	mpu_calib1.mean_ax = acc_struct1.ax / mean_denominator;
        	mpu_calib1.mean_ay = acc_struct1.ay / mean_denominator;
        	mpu_calib1.mean_az = acc_struct1.az / mean_denominator;
        	mpu_calib1.mean_gx = acc_struct1.gx / mean_denominator;
        	mpu_calib1.mean_gy = acc_struct1.gy / mean_denominator;
        	mpu_calib1.mean_gz = acc_struct1.gz / mean_denominator;
		    acc_struct1.ax = 0 ;
		    acc_struct1.ay = 0;
		    acc_struct1.az = 0 ;
		    acc_struct1.gx = 0 ;
		    acc_struct1.gy = 0;
		    acc_struct1.gz = 0 ;

        }

        else if (count > mean_period)
        {

        	// Use mean from above to calculate (ax - ux)^2 (summation term in mean)
           acc_struct1.ax  += sqr(mpu_data_cal.accel_X - mpu_calib1.mean_ax);
		   acc_struct1.ay += sqr(mpu_data_cal.accel_Y - mpu_calib1.mean_ay) ;
		   acc_struct1.az += sqr(mpu_data_cal.accel_Z - mpu_calib1.mean_az) ;
		   acc_struct1.gx += sqr(mpu_data_cal.gyro_X - mpu_calib1.mean_gx) ;
		   acc_struct1.gy += sqr(mpu_data_cal.gyro_Y - mpu_calib1.mean_gy) ;
		   acc_struct1.gz += sqr(mpu_data_cal.gyro_Z - mpu_calib1.mean_gz);
        }


        count++ ;
        if (count >= max_count)
        {

        	int var_denominator ;
        	var_denominator = var_period - mean_period - 1  ;



        	// Calculate variance

        	mpu_calib1.var_ax = acc_struct1.ax / var_denominator;
			mpu_calib1.var_ay = acc_struct1.ay / var_denominator;
			mpu_calib1.var_az = acc_struct1.az / var_denominator;
			mpu_calib1.var_gx = acc_struct1.gx / var_denominator;
			mpu_calib1.var_gy = acc_struct1.gy / var_denominator;
			mpu_calib1.var_gz = acc_struct1.gz / var_denominator;

			mpu_calib1.thresh_ax = treshold_factor*sqrt(mpu_calib1.var_ax);
			mpu_calib1.thresh_ay = treshold_factor*sqrt(mpu_calib1.var_ay);
			mpu_calib1.thresh_az = treshold_factor*sqrt(mpu_calib1.var_az);
			mpu_calib1.thresh_gx = treshold_factor*sqrt(mpu_calib1.var_gx);
			mpu_calib1.thresh_gy = treshold_factor*sqrt(mpu_calib1.var_gy);
			mpu_calib1.thresh_gz = treshold_factor*sqrt(mpu_calib1.var_gz);
        	// Assign values to stats struct


			//After completion give semaphore
			xSemaphoreGive(bin_calib_semph2);
			GPIO_Togglepin(GPIOD, 15);
			vTaskDelete(NULL);
        }
	}

}


void control_led_init(void)
{
	GPIO_Handle_t GP_D ;
	GP_D.pGPIO_Base_Addr = GPIOD ;
	GP_D.pinConfig.GPIO_PinNumber = 12 ;
	GP_D.pinConfig.GPIO_PinMode = GPIO_MODE_OUT ;
	GP_D.pinConfig.GPIO_PinOPType =  GPIO_OP_PP ;
	GPIO_ClkCtrl(GPIOD,ENABLE);   // enable gpio A in RCC
	GPIO_Init(&GP_D);
	GP_D.pinConfig.GPIO_PinNumber = 13 ;
	GPIO_Init(&GP_D);
	GP_D.pinConfig.GPIO_PinNumber = 15 ; // blue LED
    GPIO_Init(&GP_D);
}




int main(void)
{

//	clock_set_HSE();
	control_led_init();

	mpu1.which_I2C = I2C1 ;
	mpu1.dev_addr = device_address_0 ;
    mpu1.I2C_speed = MPU_SPEED_100KHZ ;

	mpu6050_init(&mpu1) ;

	rawdata_queue = xQueueCreate(6,sizeof(mpu_rawdata_t));
	bin_semph1 = xSemaphoreCreateBinary(); // from task_process to task_act
	bin_calib_semph2 = xSemaphoreCreateBinary();
 // start_TIM2();




    if(xTaskCreate(task_mpu_acquire, "acquisition", 256, NULL, 5, NULL) != pdPASS)  // pdPASS : indicates that task has been created successfully
    {
    	while(1); // task creation failed
    }
    if(xTaskCreate(task_process, "process", 256, NULL, 4, NULL) != pdPASS)
    {
    	while(1);
    }

    if(xTaskCreate(task_calibrate,"calibrate",512,NULL,4,NULL) != pdPASS)
    {
    	while(1);
    }

    if(xTaskCreate(task_act, "act", 256, NULL, 3, NULL) != pdPASS)
	{
		while(1);
	}

    vTaskStartScheduler();
    /* If you reach here → scheduler FAILED */
    // Normally,code after vTaskStartScheduler() is NEVER executed
    // Only executed if scheduler fails to start
    while(1);

}





void process_data(mpu_rawdata_t *data ,mpu_calibration_stats_t *stats, uint8_t level )
{

	int adjusted_data  ;

	adjusted_data = data->gyro_X - stats->mean_gx ;
	if( mod(adjusted_data) > stats->thresh_gx)
	{
		global_processed_data.degrees_sec_gx = adjusted_data/131.0 ;  // 1 degree means reading of 131
		global_processed_data.degrees_gx += global_processed_data.degrees_sec_gx*0.01;

	}
	  if (level > 0)
	  {

	adjusted_data = data->gyro_Y - stats->mean_gy ;
	if( mod(adjusted_data) > stats->thresh_gy)
	{
		global_processed_data.degrees_sec_gy = adjusted_data/131.0 ;  // 1 degree means reading of 131
		global_processed_data.degrees_gy += global_processed_data.degrees_sec_gy*0.01;

	}

    adjusted_data = data->gyro_Z - stats->mean_gz ;
	if( mod(adjusted_data) > stats->thresh_gz)
	{
		global_processed_data.degrees_sec_gz = adjusted_data/131.0 ;  // 1 degree means reading of 131
		global_processed_data.degrees_gz += global_processed_data.degrees_sec_gz*0.01;

	}

	adjusted_data = data->accel_X - stats->mean_ax ;
	if( mod(adjusted_data) > stats->thresh_ax)
	{
		global_processed_data.accel_ax = adjusted_data/16384.0 ;  // 1 degree means reading of 131
		global_processed_data.vel_ax += global_processed_data.accel_ax*0.01;

	}

	adjusted_data = data->accel_Y - stats->mean_ay ;
	if( mod(adjusted_data) > stats->thresh_ay)
	{
		global_processed_data.accel_ay = adjusted_data/16384.0 ;  // 1 degree means reading of 131
		global_processed_data.vel_ay += global_processed_data.accel_ay*0.01;

	}

	adjusted_data = data->accel_Z - stats->mean_az ;
	if( mod(adjusted_data) > stats->thresh_az)
	{
		global_processed_data.accel_az = adjusted_data/16384.0 ;  // 1 degree means reading of 131
		global_processed_data.vel_az += global_processed_data.accel_az*0.01;

	}

  }


}


int sqr(int16_t num)
{
	int result ;
	result = num * num ;
	return result ;
}

int mod(int num)
{

	if(num > 0)
		return num ;
	else if(num < 0)
		return num*(-1) ;
	else
		return 0 ;
}
