#include "dvr.h"
#include "key.h"
#include <LCD_ST7735S.h>
// 定义用于电源电压采样和摇杆输入的ADC通道
#define POWER_ADC_CHANNEL      ADC_Channel_3  // PA3
#define JOYSTICK_THR_ADC_CHANNEL ADC_Channel_0  // PA0
#define JOYSTICK_X_ADC_CHANNEL ADC_Channel_1  // PA1
#define JOYSTICK_Y_ADC_CHANNEL ADC_Channel_2  // PA2
#define JOYSTICK_Z_ADC_CHANNEL ADC_Channel_9  // PB1

#define VREF                   3.3f           // 参考电压，单位：V
#define ADC_RESOLUTION         4096.0f        // ADC分辨率（12位为4096）

// 定义不同通道的采样时间
#define SAMPLE_TIME_PA3        ADC_SampleTime_239Cycles5 // PA3 电源电压
#define SAMPLE_TIME_PA0        ADC_SampleTime_239Cycles5  // PA0 摇杆X轴
#define SAMPLE_TIME_PA1        ADC_SampleTime_71Cycles5  // PA1 摺杆Y轴
#define SAMPLE_TIME_PA2        ADC_SampleTime_71Cycles5  // PA2 摺杆Z轴
#define SAMPLE_TIME_PB1        ADC_SampleTime_71Cycles5  // PB1 摺杆Z轴

Dvr_TX Dvr;
_Filter Filter_Pitch, Filter_Roll, Filter_Yaw, Filter_Thr;
_offset offset;
/**
 * @brief 初始化ADC1外设，并配置PA0、PA1、PA2、PA3、PB1为模拟输入
 */
void Power_Init(void) {
    ADC_InitTypeDef ADC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 启用ADC1和GPIOA的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    
    // 配置PA3为模拟输入（用于电压采样）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    // 配置PA0, PA1, PA2为模拟输入（用于摇杆输入）
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 配置ADC1的基本参数
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;              // 独立ADC模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                   // 禁用扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;             // 禁用连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 无外部触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;          // 数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;                         // 转换通道数量
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // 启用ADC1
    ADC_Cmd(ADC1, ENABLE);
    
    // 复位并开始ADC校准
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));  // 等待复位校准完成
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));       // 等待校准完成
}

/**
 * @brief 读取指定ADC通道的转换值
 * @param channel 要读取的ADC通道
 * @param sampleTime 采样时间
 * @return 返回ADC通道的转换结果
 */
uint16_t Read_ADC(uint8_t channel, uint8_t sampleTime) {
    // 配置ADC通道和采样时间
    ADC_RegularChannelConfig(ADC1, channel, 1, sampleTime);
    
    // 启动ADC转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // 等待转换完成
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    // 返回ADC转换结果
    return ADC_GetConversionValue(ADC1);
}

/**
 * @brief 获取PA3的电源电压的ADC值
 * @return 返回采样的电源电压的ADC值
 */
uint16_t Get_PowerVoltage_ADC(void) {
    // 使用为PA3配置的采样时间
    return Read_ADC(POWER_ADC_CHANNEL, SAMPLE_TIME_PA3);
}

/**
 * @brief 获取PA0的摇杆的ADC值
 * @return 返回摇杆油门的ADC转换结果
 */
uint16_t Get_Joystick_THR_ADC(void) {
    // 使用为PA0配置的采样时间
    return Read_ADC(JOYSTICK_THR_ADC_CHANNEL, SAMPLE_TIME_PA0);
}

/**
 * @brief 获取PA1的摇杆的ADC值
 * @return 返回摇杆X轴的ADC转换结果
 */
uint16_t Get_Joystick_PIT_ADC(void) {
    // 使用为PA1配置的采样时间
    return Read_ADC(JOYSTICK_X_ADC_CHANNEL, SAMPLE_TIME_PA1);
}

/**
 * @brief 获取PA2的摇杆的ADC值
 * @return 返回摇杆Y轴的ADC转换结果
 */
uint16_t Get_Joystick_ROL_ADC(void) {
    // 使用为PA2配置的采样时间
    return Read_ADC(JOYSTICK_Y_ADC_CHANNEL, SAMPLE_TIME_PA2);
}

/**
 * @brief 获取PA2的摇杆的ADC值
 * @return 返回摇杆Y轴的ADC转换结果
 */
uint16_t Get_Joystick_YAW_ADC(void) {
    // 使用为PA2配置的采样时间
    return Read_ADC(JOYSTICK_Z_ADC_CHANNEL, SAMPLE_TIME_PB1);
}	

/**
 * @brief 将ADC值转换为实际电压值
 * @param adc_value ADC采样值
 * @return 实际的电压值（单位：V）
 */
float Convert_ADC_To_Voltage(uint16_t adc_value) {
    // 将ADC值转换为实际电压值
    return (adc_value * VREF) / ADC_RESOLUTION;
}
/*
* @description:滑动窗口滤波
* @return
*/
void window_field(Dvr_TX *dvr)
{
		static uint16_t count = 0;
		//丢掉旧数据
		Filter_Thr.sum	 -= Filter_Thr.old[count];
		Filter_Pitch.sum -= Filter_Pitch.old[count];
		Filter_Roll.sum  -= Filter_Roll.old[count];
		Filter_Yaw.sum   -= Filter_Yaw.old[count];
		//加上新数据同时保存到LOD里面
		Filter_Thr.old[count]	  = dvr->THR;
		Filter_Pitch.old[count] = dvr->PIT;
		Filter_Roll.old[count]  = dvr->ROL;
		Filter_Yaw.old[count]		= dvr->YAW;
	
		Filter_Thr.sum 	 += Filter_Thr.old[count];
		Filter_Pitch.sum += Filter_Pitch.old[count];
		Filter_Roll.sum  += Filter_Roll.old[count];
		Filter_Yaw.sum   += Filter_Yaw.old[count];
		//求平均值
		dvr->THR = Filter_Thr.sum / Filter_Number;
		dvr->PIT = Filter_Pitch.sum / Filter_Number;
		dvr->ROL = Filter_Roll.sum / Filter_Number;
		dvr->YAW = Filter_Yaw.sum / Filter_Number;
		count++;
		//判断索引范围
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
						/* 校准：偏差值 = 50次的和/50-1500 */
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
* @description:摇杆扫描
* @return
*/
void ADC_DVR_SCAN(void)
{

		/* 转换1000-2000，并处理极性	*/
		Dvr.THR = 1000 + 0.25 * (4095 - Get_Joystick_THR_ADC()) - offset.THR;
		Dvr.PIT = 1000 + 0.25*(Get_Joystick_PIT_ADC()) - offset.PIT;
		Dvr.ROL = 1000 + 0.25*(Get_Joystick_ROL_ADC()) - offset.ROL;
		Dvr.YAW = 1000 + 0.25*(Get_Joystick_YAW_ADC()) - offset.YAW;
	
//		Dvr.THR = Get_Joystick_THR_ADC();
//		Dvr.PIT = Get_Joystick_PIT_ADC();
//		Dvr.ROL = Get_Joystick_ROL_ADC();
//		Dvr.YAW = Get_Joystick_YAW_ADC();
	
		/* 滑动窗口滤波:数值变化平缓 */
		window_field(&Dvr);
		/* 油门最低标准，其他中值校准 */
		
		//Mid_offset(&Dvr);
	
		/* 限幅 */
		Dvr_Limit(&Dvr);
		/* 终点限幅 */
		Dvr_Mid_Limit(&Dvr);
	
}



