#include "power.h"


// 用于存储 ADC 采样结果的变量
static float filtered_voltage = 0.0f;  // 滤波后的电压值
static float filter_alpha = 0.1f;      // 滤波器的系数

// 初始化 ADC
void Power_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // 启用 GPIOA 和 ADC1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    // 配置 PA3 为模拟输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 配置 ADC1
    ADC_DeInit(ADC1);  // 复位 ADC1 以确保配置时状态一致
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // 独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;  // 单通道模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // 禁用连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // 无外部触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // 数据对齐方式
    ADC_InitStructure.ADC_NbrOfChannel = 1;  // 只使用一个通道
    ADC_Init(ADC1, &ADC_InitStructure);

    // 配置 ADC 通道 3 (对应 PA3) 的采样时间
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);

    // 启用 ADC1
    ADC_Cmd(ADC1, ENABLE);
    
    // 校准 ADC1
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));  // 等待校准复位完成
    ADC_StartCalibration(ADC1);  // 启动校准
    while (ADC_GetCalibrationStatus(ADC1));  // 等待校准完成
}

// 启动 ADC 转换并读取数据
uint16_t ADC_Read(void) {
    // 启动 ADC 转换
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // 等待转换完成
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    // 获取转换结果
    return ADC_GetConversionValue(ADC1);
}

// 将 ADC 值转换为实际电压值
float ADC_ConvertToVoltage(uint16_t adc_value) {
    // 假设参考电压为 3.3V，ADC 分辨率为 12 位 (0-4095)
    float v_out = (float)adc_value * 3.3f / 4096.0f;

    // 根据分压比计算实际电池电压，分压比为 1/0.638
    float battery_voltage = v_out / 0.638f;

    return battery_voltage;
}

// 初始化滤波器
void Filter_Init_Power(float alpha) {
    filter_alpha = alpha;
}

// 通过滤波器处理 ADC 采样值
float Filter_Process_Power(float new_value) {
    // 应用 IIR 滤波器
    filtered_voltage = filter_alpha * new_value + (1 - filter_alpha) * filtered_voltage;
    return filtered_voltage;
}

