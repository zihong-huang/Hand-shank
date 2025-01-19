#include "stm32f10x.h"
#include "LED.h"
#include "delay.h"

void LED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//Init_gpioB
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = RGB_R | RGB_G | RGB_B;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}


void LED_R(void)
{
		GPIO_ResetBits(GPIOB, RGB_R);
		delay_ms(20);
		GPIO_SetBits(GPIOB, RGB_R);
		delay_ms(20);
}


