#include "dvr.h"
#include "key.h"
#include <LCD_ST7735S.h>
// �������ڵ�Դ��ѹ������ҡ�������ADCͨ��
#define POWER_ADC_CHANNEL      ADC_Channel_3  // PA3
#define JOYSTICK_THR_ADC_CHANNEL ADC_Channel_0  // PA0
#define JOYSTICK_X_ADC_CHANNEL ADC_Channel_1  // PA1
#define JOYSTICK_Y_ADC_CHANNEL ADC_Channel_2  // PA2
#define JOYSTICK_Z_ADC_CHANNEL ADC_Channel_9  // PB1

#define VREF                   3.3f           // �ο���ѹ����λ��V
#define ADC_RESOLUTION         4096.0f        // ADC�ֱ��ʣ�12λΪ4096��

// ���岻ͬͨ���Ĳ���ʱ��
#define SAMPLE_TIME_PA3        ADC_SampleTime_239Cycles5 // PA3 ��Դ��ѹ
#define SAMPLE_TIME_PA0        ADC_SampleTime_239Cycles5  // PA0 ҡ��X��
#define SAMPLE_TIME_PA1        ADC_SampleTime_71Cycles5  // PA1 ߡ��Y��
#define SAMPLE_TIME_PA2        ADC_SampleTime_71Cycles5  // PA2 ߡ��Z��
#define SAMPLE_TIME_PB1        ADC_SampleTime_71Cycles5  // PB1 ߡ��Z��

Dvr_TX Dvr;
_Filter Filter_Pitch, Filter_Roll, Filter_Yaw, Filter_Thr;
_offset offset;
/**
 * @brief ��ʼ��ADC1���裬������PA0��PA1��PA2��PA3��PB1Ϊģ������
 */
void Power_Init(void) {
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // ����ADC1��GPIOA��ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    // ����PA3Ϊģ�����루���ڵ�ѹ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // ����PA0, PA1, PA2Ϊģ�����루����ҡ�����룩
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // ����ADC1�Ļ�������
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;              // ����ADCģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                   // ����ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;             // ��������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // ���ⲿ����
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;          // �����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;                         // ת��ͨ������
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // ����ADC1
    ADC_Cmd(ADC1, ENABLE);
    
    // ��λ����ʼADCУ׼
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));  // �ȴ���λУ׼���
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));       // �ȴ�У׼���
}

/**
 * @brief ��ȡָ��ADCͨ����ת��ֵ
 * @param channel Ҫ��ȡ��ADCͨ��
 * @param sampleTime ����ʱ��
 * @return ����ADCͨ����ת�����
 */
uint16_t Read_ADC(uint8_t channel, uint8_t sampleTime) {
    // ����ADCͨ���Ͳ���ʱ��
    ADC_RegularChannelConfig(ADC1, channel, 1, sampleTime);
    
    // ����ADCת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // �ȴ�ת�����
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    // ����ADCת�����
    return ADC_GetConversionValue(ADC1);
}

/**
 * @brief ��ȡPA3�ĵ�Դ��ѹ��ADCֵ
 * @return ���ز����ĵ�Դ��ѹ��ADCֵ
 */
uint16_t Get_PowerVoltage_ADC(void) {
    // ʹ��ΪPA3���õĲ���ʱ��
    return Read_ADC(POWER_ADC_CHANNEL, SAMPLE_TIME_PA3);
}

/**
 * @brief ��ȡPA0��ҡ�˵�ADCֵ
 * @return ����ҡ�����ŵ�ADCת�����
 */
uint16_t Get_Joystick_THR_ADC(void) {
    // ʹ��ΪPA0���õĲ���ʱ��
    return Read_ADC(JOYSTICK_THR_ADC_CHANNEL, SAMPLE_TIME_PA0);
}

/**
 * @brief ��ȡPA1��ҡ�˵�ADCֵ
 * @return ����ҡ��X���ADCת�����
 */
uint16_t Get_Joystick_PIT_ADC(void) {
    // ʹ��ΪPA1���õĲ���ʱ��
    return Read_ADC(JOYSTICK_X_ADC_CHANNEL, SAMPLE_TIME_PA1);
}

/**
 * @brief ��ȡPA2��ҡ�˵�ADCֵ
 * @return ����ҡ��Y���ADCת�����
 */
uint16_t Get_Joystick_ROL_ADC(void) {
    // ʹ��ΪPA2���õĲ���ʱ��
    return Read_ADC(JOYSTICK_Y_ADC_CHANNEL, SAMPLE_TIME_PA2);
}

/**
 * @brief ��ȡPA2��ҡ�˵�ADCֵ
 * @return ����ҡ��Y���ADCת�����
 */
uint16_t Get_Joystick_YAW_ADC(void) {
    // ʹ��ΪPA2���õĲ���ʱ��
    return Read_ADC(JOYSTICK_Z_ADC_CHANNEL, SAMPLE_TIME_PB1);
}	

/**
 * @brief ��ADCֵת��Ϊʵ�ʵ�ѹֵ
 * @param adc_value ADC����ֵ
 * @return ʵ�ʵĵ�ѹֵ����λ��V��
 */
float Convert_ADC_To_Voltage(uint16_t adc_value) {
    // ��ADCֵת��Ϊʵ�ʵ�ѹֵ
    return (adc_value * VREF) / ADC_RESOLUTION;
}
/*
* @description:���������˲�
* @return
*/
void window_field(Dvr_TX *dvr)
{
		static uint16_t count = 0;
		//����������
		Filter_Thr.sum	 -= Filter_Thr.old[count];
		Filter_Pitch.sum -= Filter_Pitch.old[count];
		Filter_Roll.sum  -= Filter_Roll.old[count];
		Filter_Yaw.sum   -= Filter_Yaw.old[count];
		//����������ͬʱ���浽LOD����
		Filter_Thr.old[count]	  = dvr->THR;
		Filter_Pitch.old[count] = dvr->PIT;
		Filter_Roll.old[count]  = dvr->ROL;
		Filter_Yaw.old[count]		= dvr->YAW;
	
		Filter_Thr.sum 	 += Filter_Thr.old[count];
		Filter_Pitch.sum += Filter_Pitch.old[count];
		Filter_Roll.sum  += Filter_Roll.old[count];
		Filter_Yaw.sum   += Filter_Yaw.old[count];
		//��ƽ��ֵ
		dvr->THR = Filter_Thr.sum / Filter_Number;
		dvr->PIT = Filter_Pitch.sum / Filter_Number;
		dvr->ROL = Filter_Roll.sum / Filter_Number;
		dvr->YAW = Filter_Yaw.sum / Filter_Number;
		count++;
		//�ж�������Χ
		if(count >= 10)		{
				count = 0;
		}
	
	
}	

/*
* @description:Dvr_Limit
* @return
*/
void Dvr_Limit(Dvr_TX *dvr)
{
		dvr->THR = (dvr->THR < 1000)?1000:dvr->THR;
		dvr->THR = (dvr->THR > 2000)?2000:dvr->THR;
	
		dvr->PIT = (dvr->PIT < 1000)?1000:dvr->PIT;
		dvr->PIT = (dvr->PIT > 2000)?2000:dvr->PIT;
	
		dvr->ROL = (dvr->ROL < 1000)?1000:dvr->ROL;
		dvr->ROL = (dvr->ROL > 2000)?2000:dvr->ROL;
	
		dvr->YAW = (dvr->YAW < 1000)?1000:dvr->YAW;
		dvr->YAW = (dvr->YAW > 2000)?2000:dvr->YAW;
}

/*
* @description:Dvr_Mid_Limit
* @return
*/
void Dvr_Mid_Limit(Dvr_TX *dvr)
{
		if(dvr->THR > 1450 && dvr->THR < 1550)
		{
				dvr->THR = 1500;
		}
		
		if(dvr->PIT > 1490 && dvr->PIT < 1510)
		{
				dvr->PIT = 1500;
		}
		
		if(dvr->ROL > 1490 && dvr->ROL < 1510)
		{
				dvr->ROL = 1500;
		}
		if(dvr->YAW > 1490 && dvr->YAW < 1510)
		{
				dvr->YAW = 1500;
		}
		
}

/*
* @description:mid_offset
* @return
*/
void Mid_offset(Dvr_TX *dvr)
{
		static uint16_t key_count = 0;
		static uint16_t	sum_thr=0,sum_roll=0,sum_pit=0,sum_yaw=0, count=0;
		if(!Key_con.key3_flasg)
		{
				key_count++;
				if(key_count>=20)
				{
						/* У׼��ƫ��ֵ = 50�εĺ�/50-1500 */
						if(count == 0)
						{
								sum_thr=0;
								sum_roll=0;
								sum_pit=0;
								sum_yaw=0;
								offset.THR = 0;
								offset.PIT = 0;
								offset.ROL = 0;
								offset.YAW = 0;
								count = 1;
						}else{
								
								sum_thr=	 dvr->THR;
								sum_roll=	 dvr->ROL;
								sum_pit=	 dvr->PIT;
								sum_yaw=	 dvr->YAW;
								count++;
						}
						if(count == 51)
						{
								count--;
								offset.THR = sum_thr/count -1000;
								offset.ROL = sum_roll/count -1500;
								offset.PIT = sum_pit/count -1500;
								offset.YAW = sum_yaw/count -1500;
								count = 0;
								key_count = 0;
						}
							
						
				}
		}
		
}

/*
* @description:ҡ��ɨ��
* @return
*/
void ADC_DVR_SCAN(void)
{

		/* ת��1000-2000����������	*/
		Dvr.THR = 1000 + 0.25 * (4095 - Get_Joystick_THR_ADC()) - offset.THR;
		Dvr.PIT = 1000 + 0.25*(Get_Joystick_PIT_ADC()) - offset.PIT;
		Dvr.ROL = 1000 + 0.25*(Get_Joystick_ROL_ADC()) - offset.ROL;
		Dvr.YAW = 1000 + 0.25*(Get_Joystick_YAW_ADC()) - offset.YAW;
	
//		Dvr.THR = Get_Joystick_THR_ADC();
//		Dvr.PIT = Get_Joystick_PIT_ADC();
//		Dvr.ROL = Get_Joystick_ROL_ADC();
//		Dvr.YAW = Get_Joystick_YAW_ADC();
	
		/* ���������˲�:��ֵ�仯ƽ�� */
		window_field(&Dvr);
		/* ������ͱ�׼��������ֵУ׼ */
		
		//Mid_offset(&Dvr);
	
		/* �޷� */
		Dvr_Limit(&Dvr);
		/* �յ��޷� */
		Dvr_Mid_Limit(&Dvr);
	
}



