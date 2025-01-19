#include "power.h"


// ���ڴ洢 ADC ��������ı���
static float filtered_voltage = 0.0f;  // �˲���ĵ�ѹֵ
static float filter_alpha = 0.1f;      // �˲�����ϵ��

// ��ʼ�� ADC
void Power_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    // ���� GPIOA �� ADC1 ʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    // ���� PA3 Ϊģ������ģʽ
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // ���� ADC1
    ADC_DeInit(ADC1);  // ��λ ADC1 ��ȷ������ʱ״̬һ��
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;  // ����ģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;  // ��ͨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // ��������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  // ���ⲿ����
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  // ���ݶ��뷽ʽ
    ADC_InitStructure.ADC_NbrOfChannel = 1;  // ֻʹ��һ��ͨ��
    ADC_Init(ADC1, &ADC_InitStructure);

    // ���� ADC ͨ�� 3 (��Ӧ PA3) �Ĳ���ʱ��
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5);

    // ���� ADC1
    ADC_Cmd(ADC1, ENABLE);
    
    // У׼ ADC1
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));  // �ȴ�У׼��λ���
    ADC_StartCalibration(ADC1);  // ����У׼
    while (ADC_GetCalibrationStatus(ADC1));  // �ȴ�У׼���
}

// ���� ADC ת������ȡ����
uint16_t ADC_Read(void) {
    // ���� ADC ת��
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
    // �ȴ�ת�����
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    
    // ��ȡת�����
    return ADC_GetConversionValue(ADC1);
}

// �� ADC ֵת��Ϊʵ�ʵ�ѹֵ
float ADC_ConvertToVoltage(uint16_t adc_value) {
    // ����ο���ѹΪ 3.3V��ADC �ֱ���Ϊ 12 λ (0-4095)
    float v_out = (float)adc_value * 3.3f / 4096.0f;

    // ���ݷ�ѹ�ȼ���ʵ�ʵ�ص�ѹ����ѹ��Ϊ 1/0.638
    float battery_voltage = v_out / 0.638f;

    return battery_voltage;
}

// ��ʼ���˲���
void Filter_Init_Power(float alpha) {
    filter_alpha = alpha;
}

// ͨ���˲������� ADC ����ֵ
float Filter_Process_Power(float new_value) {
    // Ӧ�� IIR �˲���
    filtered_voltage = filter_alpha * new_value + (1 - filter_alpha) * filtered_voltage;
    return filtered_voltage;
}

