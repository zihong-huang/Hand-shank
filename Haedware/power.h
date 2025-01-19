#ifndef __POWER_H
#define __POWER_H

#include "sys.h"


// ��ʼ�� ADC
void Power_Init(void);

// ���� ADC ת��
uint16_t ADC_Read(void);

// �� ADC ֵת��Ϊʵ�ʵ�ѹֵ
float ADC_ConvertToVoltage(uint16_t adc_value);

// ��ʼ���˲���
void Filter_Init_Power(float alpha);

// ͨ���˲������� ADC ����ֵ
float Filter_Process_Power(float new_value);


#endif
