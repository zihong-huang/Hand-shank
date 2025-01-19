#ifndef __MYSPI_H
#define __MYSPI_H
#include "sys.h"
 
void MySPI_Init(void);
void MySPI_Start(void);
void MySPI_Stop(void);



void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);
uint8_t MySPI_SwapByte(uint8_t ByteSend);
 
#endif
