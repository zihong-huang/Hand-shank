#include<stdio.h>
#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "freertos_demo.h"

#include "delay.h"
#include "LED.h"
#include "LCD_ST7735S.h"
#include "key.h"
#include "nrf24l01.h"
#include "dvr.h"
#include "power.h"

//��̬����NRF_TX
#define START_TASK_NRF_TX  2						//�������ȼ�
#define START_TX_SIZE	    128				  	//��ջ��С
TaskHandle_t	Task_NRF_TX_Handle;				//������

//��̬����NRF_RX
#define START_TASK_NRF_RX  2						//�������ȼ�
#define START_RX_SIZE			128				 	  //��ջ��С
TaskHandle_t	Task_NRF_RX_Handle;				//������


//��̬����DVR
#define START_TASK_DVR 		2						  //�������ȼ�
#define START_DVR_SIZE		128						//��ջ��С
TaskHandle_t	Task_DVR_Handle;					//������

//��̬����KEY
#define START_TASK_KEY 		2					  	//�������ȼ�
#define START_KEY_SIZE		64						//��ջ��С
TaskHandle_t	Task_KEY_Handle;					//������

int main(void)
{

//�����ʼ��	
	delay_init();
	lcdInit();
	Key_Init();
	Init_NRF24L01();
	lcdClear(GRAYBLUE);
  Power_Init();        
	LCD_ShowString(10, 10, 10, 10, 12, (u8*)"OK", GREEN, GRAYBLUE);
	if(NRF24L01_Check() == 0)
	LCD_ShowString(10, 30, 50, 10, 12, (u8*)"NRF_OK", GREEN, GRAYBLUE);
	else
	LCD_ShowString(10, 30, 50, 10, 12, (u8*)"NRF_ER", GREEN, GRAYBLUE);
	lcdClear(GRAYBLUE);

	xQueue_Send_Data = xQueueCreate(3, sizeof(char)*13);
	
	xTaskCreate((TaskFunction_t)	Task_NRF_TX,					//����NRF�߳�
						(char *)					"Task_NRF_TX",
						(uint16_t)				START_TX_SIZE,
						(void *) 					NULL,
						(UBaseType_t)			START_TASK_NRF_TX,
						(TaskHandle_t *)	&Task_NRF_TX_Handle );
						
//	xTaskCreate((TaskFunction_t)	Task_NRF_RX,					//����NRF�߳�
//						(char *)					"Task_NRF_RX",
//						(uint16_t)				START_RX_SIZE,
//						(void *) 					NULL,
//						(UBaseType_t)			START_TASK_NRF_RX,
//						(TaskHandle_t *)	&Task_NRF_RX_Handle );
	
	
	xTaskCreate((TaskFunction_t)	Task_Dvr_Get_data,					//����Dvr�߳�
						(char *)					"Task_Dvr_Get_data",
						(uint16_t)				START_DVR_SIZE,
						(void *) 					NULL,
						(UBaseType_t)			START_TASK_DVR,
						(TaskHandle_t *)	&Task_DVR_Handle );	
						
//	xTaskCreate((TaskFunction_t)	KEY_Scan,					//����KEY�߳�
//						(char *)					"Task_KEY",
//						(uint16_t)				START_KEY_SIZE,
//						(void *) 					NULL,
//						(UBaseType_t)			START_TASK_KEY,
//						(TaskHandle_t *)	&Task_KEY_Handle );	
						


//	SemapMTX_Handle_t = xSemaphoreCreateBinary();
//	SemapMRX_Handle_t = xSemaphoreCreateBinary();
//	xSemaphoreGive(SemapMTX_Handle_t);
						
	vTaskStartScheduler();
	

	
	while(1)
	{


		

	}
}



