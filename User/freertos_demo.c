#include "freertos_demo.h"

//DVR���Ͷ���
QueueHandle_t xQueue_Send_Data;

//������ֵ�ź���
//SemaphoreHandle_t SemapMTX_Handle_t;//���������ֵ�ź������
//SemaphoreHandle_t SemapMRX_Handle_t;//���������ֵ�ź������

_Show_flag Show;

void Task_NRF_TX(void)
{

	
		char Receive_Send_Data[13];
		BaseType_t Send_Data;
	
		while(1)
		{
//				while(xSemaphoreTake(SemapMTX_Handle_t,portMAX_DELAY))
//				{
								TX_Mode();

								Send_Data = xQueueReceive(xQueue_Send_Data, &Receive_Send_Data, portMAX_DELAY);
								if(Send_Data != pdPASS)
								{
										Show.Queue_flag = 0x01;
								}
								//��������
								if(NRF24L01_TxPacket((u8*)Receive_Send_Data) == TX_OK)
								Show.Succeeful_flag = 0x01;
								else Show.Error_flag = 0x01;
								
								//�������
								memset(Receive_Send_Data, 0, sizeof(Receive_Send_Data));		
								

//						xSemaphoreGive(SemapMRX_Handle_t);
//				}
				vTaskDelay(pdMS_TO_TICKS(2));
		}
	
}

void Task_NRF_RX(void)
{
		u8 i;
		char arr[8];
		char rx_arr[7];
	
//		int Si;
//		char analyze[4];
		while(1)
		{
//				while(xSemaphoreTake(SemapMRX_Handle_t, portMAX_DELAY))
//				{
							RX_Mode();
							delay_ms(2);
							if(NRF24L01_RxPacket((u8*)arr) == 0)
							{
								
									for(i = 0; i < 8; i++)
									{
										rx_arr[i] = arr[i+1];
										
									}
									Analyze_data(rx_arr);
										
							}
//					xSemaphoreGive(SemapMTX_Handle_t);
//				}
				vTaskDelay(pdMS_TO_TICKS(2));
			}

		
}

void Task_Dvr_Get_data(void)
{

		while(1)
		{

				ADC_DVR_SCAN();
				Send_ADC_Data();
//				taskENTER_CRITICAL();
				//���ϳɹ�״̬��
//				if(Show.Succeeful_flag == 0x01){
//						LCD_ShowString(10, 10, 20, 10,12, (u8*)"su", GREEN, GRAYBLUE);
//				}
//				//���´������
//			
//				//����dvr����
//				LCD_ShowIntNum(100,10, Dvr.THR, 5, GREEN,GRAYBLUE, 12);
//				LCD_ShowIntNum(100,20, Dvr.PIT, 5, GREEN,GRAYBLUE, 12);
//				LCD_ShowIntNum(100,30, Dvr.ROL, 5, GREEN,GRAYBLUE, 12);
//				LCD_ShowIntNum(100,40, Dvr.YAW, 5, GREEN,GRAYBLUE, 12);
//				
//				taskEXIT_CRITICAL();
				vTaskDelay(pdMS_TO_TICKS(50));
		}
		
}

//Send data
void Send_ADC_Data(void)
{
		char send_data[13];
		BaseType_t status;
		//д�봫�䳤��
		send_data[0] = 10;
		//֡ͷ
		send_data[1] = 0xFD;
		send_data[2] = 0xFD;
		//��������
		send_data[3] = (Dvr.THR >> 8) & 0xFF;
		send_data[4] = Dvr.THR & 0xFF;
		//X��
		send_data[5] = (Dvr.PIT >> 8) & 0xFF;
		send_data[6] = Dvr.PIT & 0xFF;
		//Y��
		send_data[7] = (Dvr.ROL >> 8) & 0xFF;
		send_data[8] = Dvr.ROL & 0xFF;
		//Y��
		send_data[9] = (Dvr.YAW >> 8) & 0xFF;
		send_data[10] = Dvr.YAW & 0xFF;
		//����
		if(Key_con.key1_flasg == 1)
		{
				LED_R();
				send_data[11] = 0x06;
		}
		else
		send_data[11] = 0x00;
		//֡β
		send_data[12] = 0xFE;
		status = xQueueSend(xQueue_Send_Data, (void*)send_data, portMAX_DELAY);
		if(status != pdPASS)
		{
				Show.Queue_flag = 0x02;
		}
}



void KEY_Scan(void)
{
		Key_con.key1_flasg = 0;
		Key_con.key2_flasg = 0;
		while(1)
		{
				if(KEY1 == 0)
				Key_con.key1_flasg = 1;
				else
				Key_con.key1_flasg = 0;
				
				if(KEY2 == 0){
					Key_con.key2_flasg = 1;
					//lcdClear(GRAYBLUE);

				}
				else {
					Key_con.key2_flasg = 0;
					//lcdClear(GRAYBLUE);
				}
				
				vTaskDelay(pdMS_TO_TICKS(2));
				
		}
	
}

	



