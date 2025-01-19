#ifndef __KEY_H
#define __KEY_H

#include "sys.h"


#define KEY3 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)  // ��ȡ����3		K3
#define KEY2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)  // ��ȡ����4		K2
#define KEY1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)  // ��ȡ����5		K1
#define SW1  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10) // ��ȡ����9		SW1
#define SW2  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13) // ��ȡ����10		SW2

typedef struct Key_state{
	u8 key1_flasg;
	u8 key2_flasg;
	u8 key3_flasg;
	u8 sw1_flasg;
	u8 sw2_flasg;
}Key_state_t;

extern Key_state_t Key_con;


void Key_Init(void);



#endif
