#ifndef __COMPUTE_H
#define __COMPUTE_H

#include "sys.h"
#include "LCD_ST7735S.h"
unsigned char* Float_to_Byte(float f);
extern unsigned char byte[4];

float Byte_to_Float(unsigned char *p);
int conversion(char str[]);

void Analyze_data(char arr[]);
#endif
