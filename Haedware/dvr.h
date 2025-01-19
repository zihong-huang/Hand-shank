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
// �����ӿ�����

/**
 * @brief ��ʼ��ADC1���裬������PA0��PA1��PA2��PA3Ϊģ������
 */
void Power_Init(void);

/**
 * @brief ��ȡָ��ADCͨ����ת��ֵ
 * @param channel Ҫ��ȡ��ADCͨ��
 * @param sampleTime ����ʱ��
 * @return ����ADCͨ����ת�����
 */
uint16_t Read_ADC(uint8_t channel, uint8_t sampleTime);

/**
 * @brief ��ȡPA3�ĵ�Դ��ѹ��ADCֵ
 * @return ���ز����ĵ�Դ��ѹ��ADCֵ
 */
uint16_t Get_PowerVoltage_ADC(void);

/**
 * @brief ��ȡPA0��ҡ�˵�ADCֵ
 * @return ����ҡ�����ŵ�ADCת�����
 */
uint16_t Get_Joystick_THR_ADC(void);

/**
 * @brief ��ȡPA1��ҡ�˵�ADCֵ
 * @return ����ҡ��X���ADCת�����
 */
uint16_t Get_Joystick_PIT_ADC(void);

/**
 * @brief ��ȡPA2��ҡ�˵�ADCֵ
 * @return ����ҡ��Y���ADCת�����
 */
uint16_t Get_Joystick_ROL_ADC(void);
uint16_t Get_Joystick_YAW_ADC(void);
/**
 * @brief ��ADCֵת��Ϊʵ�ʵ�ѹֵ
 * @param adc_value ADC����ֵ
 * @return ʵ�ʵĵ�ѹֵ����λ��V��
 */
float Convert_ADC_To_Voltage(uint16_t adc_value);

void ADC_DVR_SCAN(void);
void window_field(Dvr_TX *dvr);
void Mid_offset(Dvr_TX *dvr);
void Dvr_Limit(Dvr_TX *dvr);
void Dvr_Mid_Limit(Dvr_TX *dvr);


#endif 
