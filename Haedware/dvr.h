#ifndef __DVR_H
#define __DVR_H

#include "sys.h"

#define Filter_Number 8

typedef struct{
			u32 sum;
			u32 old[Filter_Number];
}_Filter;

typedef struct{
			int16_t  THR;
			int16_t  YAW;
			int16_t	 ROL;
			int16_t  PIT;
}Dvr_TX;

typedef struct{
			int16_t  THR;
			int16_t  YAW;
			int16_t	 ROL;
			int16_t  PIT;
}_offset;


extern Dvr_TX Dvr;
extern _offset offset;
// 函数接口声明

/**
 * @brief 初始化ADC1外设，并配置PA0、PA1、PA2、PA3为模拟输入
 */
void Power_Init(void);

/**
 * @brief 读取指定ADC通道的转换值
 * @param channel 要读取的ADC通道
 * @param sampleTime 采样时间
 * @return 返回ADC通道的转换结果
 */
uint16_t Read_ADC(uint8_t channel, uint8_t sampleTime);

/**
 * @brief 获取PA3的电源电压的ADC值
 * @return 返回采样的电源电压的ADC值
 */
uint16_t Get_PowerVoltage_ADC(void);

/**
 * @brief 获取PA0的摇杆的ADC值
 * @return 返回摇杆油门的ADC转换结果
 */
uint16_t Get_Joystick_THR_ADC(void);

/**
 * @brief 获取PA1的摇杆的ADC值
 * @return 返回摇杆X轴的ADC转换结果
 */
uint16_t Get_Joystick_PIT_ADC(void);

/**
 * @brief 获取PA2的摇杆的ADC值
 * @return 返回摇杆Y轴的ADC转换结果
 */
uint16_t Get_Joystick_ROL_ADC(void);
uint16_t Get_Joystick_YAW_ADC(void);
/**
 * @brief 将ADC值转换为实际电压值
 * @param adc_value ADC采样值
 * @return 实际的电压值（单位：V）
 */
float Convert_ADC_To_Voltage(uint16_t adc_value);

void ADC_DVR_SCAN(void);
void window_field(Dvr_TX *dvr);
void Mid_offset(Dvr_TX *dvr);
void Dvr_Limit(Dvr_TX *dvr);
void Dvr_Mid_Limit(Dvr_TX *dvr);


#endif 
