#ifndef __POWER_H
#define __POWER_H

#include "sys.h"


// 初始化 ADC
void Power_Init(void);

// 启动 ADC 转换
uint16_t ADC_Read(void);

// 将 ADC 值转换为实际电压值
float ADC_ConvertToVoltage(uint16_t adc_value);

// 初始化滤波器
void Filter_Init_Power(float alpha);

// 通过滤波器处理 ADC 采样值
float Filter_Process_Power(float new_value);


#endif
