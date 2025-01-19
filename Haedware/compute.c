#include "compute.h"

unsigned char byte[4];

//浮点型转十六进制
unsigned char* Float_to_Byte(float f)
{
//	float float_data = 0;
	unsigned long longdata = 0;
	longdata = *(unsigned long*)&f;           //注意，会丢失精度
	//byte[0] = 4;
	byte[0] = (longdata & 0xFF000000) >> 24;
	byte[1] = (longdata & 0x00FF0000) >> 16;
	byte[2] = (longdata & 0x0000FF00) >> 8;
	byte[3] = (longdata & 0x000000FF);
	return byte;
}

//十六进制转浮点型
float Byte_to_Float(unsigned char *p)
{
	float float_data=0;
	unsigned long longdata = 0;
	longdata = (*p<< 24) + (*(p+1) << 16) + (*(p + 2) << 8) + (*(p + 3) << 0);
	float_data = *(float*)&longdata;
	return float_data;
}

// 函数定义：将字节数组转换为整数
unsigned int BytesToInt(unsigned char arr[], int length) {
    unsigned int result = 0;
    for (int i = 0; i < length; i++) {
        result |= arr[i] << (8 * (length - 1 - i));
    }
    return result;
}



void Analyze_data(char Analyze_arr[])
{
		
		float GroyX;
		float GroyY;
		float GroyZ;
		char analyze[4];	
	
	
		if (Analyze_arr[0] == 0xFA && Analyze_arr[1] == 0xFB)
		{
				// 提取数据并存储到新的数组中
				analyze[0] = Analyze_arr[2];  // 高位字节
				analyze[1] = Analyze_arr[3];
//							analyze[2] = Check[4];
//							analyze[3] = Check[5];  // 低位字节

				// 检查帧尾
				if (Analyze_arr[6] == 0xFE)
				{
//						LCD_ShowString(10, 60, 50, 10, 12, (u8*)"su", GREEN, GRAYBLUE);
//						Si = (analyze[0] << 8) | (analyze[1]);
//						LCD_ShowIntNum(50, 50, Si, 4, GREEN, GRAYBLUE, 12);
				}
				else
				{
//						LCD_ShowString(10, 60, 50, 10, 12, (u8*)"FE", GREEN, GRAYBLUE);
				}
		}
		else
		{
//				LCD_ShowString(40, 60, 50, 10, 12, (u8*)"FH", GREEN, GRAYBLUE);
		}
		
		if (Analyze_arr[0] == 0xFE && Analyze_arr[1] == 0xFA)
		{
				// 提取数据并存储到新的数组中
				analyze[0] = Analyze_arr[2];  // 高位字节
				analyze[1] = Analyze_arr[3];
				analyze[2] = Analyze_arr[4];
				analyze[3] = Analyze_arr[5];  // 低位字节

				// 检查帧尾
				if (Analyze_arr[6] == 0xFE)
				{
//						LCD_ShowString(10, 60, 50, 10, 12, (u8*)"su", GREEN, GRAYBLUE);
						GroyX = Byte_to_Float((unsigned char*)Analyze_arr);
//						LCD_ShowFloatNum(60, 50, GroyX, 4, GREEN, GRAYBLUE, 12);
				}
				else
				{
//						LCD_ShowString(10, 60, 50, 10, 12, (u8*)"FE", GREEN, GRAYBLUE);
				}
		}
		else
		{
//				LCD_ShowString(40, 60, 50, 10, 12, (u8*)"FH", GREEN, GRAYBLUE);
		}
		
		if (Analyze_arr[0] == 0xFE && Analyze_arr[1] == 0xFB)
		{
				// 提取数据并存储到新的数组中
				analyze[0] = Analyze_arr[2];  // 高位字节
				analyze[1] = Analyze_arr[3];
				analyze[2] = Analyze_arr[4];
				analyze[3] = Analyze_arr[5];  // 低位字节

				// 检查帧尾
				if (Analyze_arr[6] == 0xFE)
				{
//						LCD_ShowString(10, 60, 50, 10, 12, (u8*)"su", GREEN, GRAYBLUE);
						GroyY = Byte_to_Float((unsigned char*)Analyze_arr);
//						LCD_ShowFloatNum(60, 80, GroyY, 4, GREEN, GRAYBLUE, 12);
				}
				else
				{
//						LCD_ShowString(10, 60, 50, 10, 12, (u8*)"FE", GREEN, GRAYBLUE);
				}
		}
		else
		{
//				LCD_ShowString(40, 60, 50, 10, 12, (u8*)"FH", GREEN, GRAYBLUE);
		}
		
		
		if (Analyze_arr[0] == 0xFE && Analyze_arr[1] == 0xFC)
		{
				// 提取数据并存储到新的数组中
				analyze[0] = Analyze_arr[2];  // 高位字节
				analyze[1] = Analyze_arr[3];
				analyze[2] = Analyze_arr[4];
				analyze[3] = Analyze_arr[5];  // 低位字节

				// 检查帧尾
				if (Analyze_arr[6] == 0xFE)
				{
//						LCD_ShowString(10, 60, 50, 10, 12, (u8*)"su", GREEN, GRAYBLUE);
						GroyZ = Byte_to_Float((unsigned char*)Analyze_arr);
//						LCD_ShowFloatNum(60, 110, GroyZ, 4, GREEN, GRAYBLUE, 12);
				}
				else
				{
//						LCD_ShowString(10, 60, 50, 10, 12, (u8*)"FE", GREEN, GRAYBLUE);
				}
		}
		else
		{
//				LCD_ShowString(40, 60, 50, 10, 12, (u8*)"FH", GREEN, GRAYBLUE);
		}
		
}

