#ifndef __FREERTOS_H
#define __FREERTOS_H

#include "sys.h"
#include "task.h"
#include "FreeRTOS.h"
#include "semphr.h"

#include "delay.h"
#include "LED.h"
#include "LCD_ST7735S.h"
#include "key.h"
#include "nrf24l01.h"
#include "dvr.h"
#include "power.h"
#include "compute.h"

//NRF
void Task_NRF_TX(void);
void Task_NRF_RX(void);

//油门摇杆消息队列
extern QueueHandle_t xQueue_Send_Data;

extern SemaphoreHandle_t SemapMTX_Handle_t;
extern SemaphoreHandle_t SemapMRX_Handle_t;


//油门摇杆
void Task_Dvr_Get_data(void);

void Throttle_data_dispose(void);
void DvrX_data_dispose(void);
void DvrY_data_dispose(void);
void Key_data_dispose(void);
void KEY_Scan(void);
void Send_ADC_Data(void);

#endif
