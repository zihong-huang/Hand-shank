#include <stm32f10x.h>
#include <LCD_ST7735S.h>
#include <delay.h>
#include <font.h>
#include <hz_16x16.h>
#include <hz_32x32.h>

void lcdGpioInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = LCD_PIN_CS | LCD_PIN_BLK;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_PORT, &GPIO_InitStructure);
		GPIO_SetBits(LCD_PORT, LCD_PIN_BLK);
	
		GPIO_InitStructure.GPIO_Pin =  LCD_PIN_SDA | LCD_PIN_SCL | LCD_PIN_RES | LCD_PIN_A0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LCD_PORT1, &GPIO_InitStructure);
	
}

void lcdInit(void)
{
    lcdGpioInit();
    
    SET_LCD_RES; 
    delay_ms(10);
		//vTaskDelay(10);
		CLR_LCD_RES; 
		delay_ms(10);
		//vTaskDelay(10);
		SET_LCD_RES; 
		delay_ms(200);
		//vTaskDelay(200);
    
    lcdSelectRegister(0x11); //Sleep out 
    delay_ms(120);   
		//vTaskDelay(120);
	
    lcdSelectRegister(0xB1); 
    lcdWriteDataU8(0x05); 
    lcdWriteDataU8(0x3C);
    lcdWriteDataU8(0x3C); 
    
    lcdSelectRegister(0xB2); 
    lcdWriteDataU8(0x05); 
    lcdWriteDataU8(0x3C); 
    lcdWriteDataU8(0x3C); 
    
    lcdSelectRegister(0xB3); 
    lcdWriteDataU8(0x05); 
    lcdWriteDataU8(0x3C); 
    lcdWriteDataU8(0x3C); 
    lcdWriteDataU8(0x05); 
    lcdWriteDataU8(0x3C); 
    lcdWriteDataU8(0x3C); 
    
    lcdSelectRegister(0xB4);
    lcdWriteDataU8(0x03); 
    
    lcdSelectRegister(0xC0); 
    lcdWriteDataU8(0x28); 
    lcdWriteDataU8(0x08); 
    lcdWriteDataU8(0x04); 
    
    lcdSelectRegister(0xC1); 
    lcdWriteDataU8(0XC0); 
    
    lcdSelectRegister(0xC2); 
    lcdWriteDataU8(0x0D); 
    lcdWriteDataU8(0x00); 
    
    lcdSelectRegister(0xC3); 
    lcdWriteDataU8(0x8D); 
    lcdWriteDataU8(0x2A); 
    
    lcdSelectRegister(0xC4); 
    lcdWriteDataU8(0x8D); 
    lcdWriteDataU8(0xEE); 
    
    lcdSelectRegister(0xC5);
    lcdWriteDataU8(0x1A); 
    
    lcdSelectRegister(0x36);
    lcdWriteDataU8(0xC0); 
    
    lcdSelectRegister(0xE0); 
    lcdWriteDataU8(0x04); 
    lcdWriteDataU8(0x22); 
    lcdWriteDataU8(0x07); 
    lcdWriteDataU8(0x0A); 
    lcdWriteDataU8(0x2E); 
    lcdWriteDataU8(0x30); 
    lcdWriteDataU8(0x25); 
    lcdWriteDataU8(0x2A); 
    lcdWriteDataU8(0x28); 
    lcdWriteDataU8(0x26); 
    lcdWriteDataU8(0x2E); 
    lcdWriteDataU8(0x3A); 
    lcdWriteDataU8(0x00); 
    lcdWriteDataU8(0x01); 
    lcdWriteDataU8(0x03); 
    lcdWriteDataU8(0x13); 
    
    lcdSelectRegister(0xE1); 
    lcdWriteDataU8(0x04); 
    lcdWriteDataU8(0x16); 
    lcdWriteDataU8(0x06); 
    lcdWriteDataU8(0x0D); 
    lcdWriteDataU8(0x2D); 
    lcdWriteDataU8(0x26); 
    lcdWriteDataU8(0x23); 
    lcdWriteDataU8(0x27); 
    lcdWriteDataU8(0x27); 
    lcdWriteDataU8(0x25); 
    lcdWriteDataU8(0x2D); 
    lcdWriteDataU8(0x3B); 
    lcdWriteDataU8(0x00); 
    lcdWriteDataU8(0x01); 
    lcdWriteDataU8(0x04); 
    lcdWriteDataU8(0x13); 
    
    lcdSelectRegister(0x3A);
    lcdWriteDataU8(0x05);
    
    lcdSelectRegister(0x29); 
		
		lcdSelectRegister(0x36);
		lcdWriteDataU8(0xde);	//rgb and bgr color swap
		
}

void lcdSelectRegister(unsigned char data)
{
    CLR_LCD_A0;
    CLR_LCD_CS;
 
    for(int i = 0; i < 8; i++)
    {
        if(data & 0x80)
        {
            SET_LCD_SDA;
        }
        else 
        {
            CLR_LCD_SDA;
        }
        CLR_LCD_SCL;
        SET_LCD_SCL;
        data = data << 1;
    }	 
    SET_LCD_CS;
}

void lcdWriteDataU8(unsigned char data)
{
    SET_LCD_A0;
    CLR_LCD_CS;

    for(int i = 0; i < 8; i++)
    {
        if( data & 0x80 )
        {
            SET_LCD_SDA;
        }
        else 
        {
            CLR_LCD_SDA;
        }
        CLR_LCD_SCL;
        SET_LCD_SCL;
        data = data << 1;
    }
    SET_LCD_CS;
}

void lcdWriteDataU16(unsigned short data)
{
    lcdWriteDataU8(data>>8);
    lcdWriteDataU8(data);
}

void lcdSetAddress(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2)
{	     
    lcdSelectRegister(0x2A); 
    lcdWriteDataU8(x1>>8); 
    lcdWriteDataU8(x1&0xFF);
    lcdWriteDataU8(x2>>8); 
    lcdWriteDataU8(x2&0xFF);

    lcdSelectRegister(0x2B); 
    lcdWriteDataU8(y1>>8); 
    lcdWriteDataU8(y1&0xFF);
    lcdWriteDataU8(y2>>8); 
    lcdWriteDataU8(y2&0xFF);
    
    lcdSelectRegister(0x2C);
}

void lcdClear(unsigned short color)
{
    lcdSetAddress(0, 0, 130, 162);
    
    for(unsigned char i = 0; i < 130; i++)
    {
        for (unsigned char j = 0; j < 162; j++)
	   	{
            lcdWriteDataU16(color);
	    }
    }
} 

//LCD画点
void LCD_DrawPoint(u16 x, u16 y, u16 color)
{
	 lcdSetAddress(x, y, x, y);//设置光标位置 
	 lcdWriteDataU16(color);
}


/******************************************************************************
			函数说明：快速画点
			入口参数：x,y					起点坐标
							 color				画圆颜色
			返回参数：无
******************************************************************************/ 
void LCD_Fast_DrawPoint(u16 x, u16 y, u16 color)
{
	lcdWriteDataU16(color);
}	

//LCD开启显示
void LCD_DisplayOn(void)
{					   
	lcdSelectRegister(0x29);	//开启显示
}	 
//LCD关闭显示
void LCD_DisplayOff(void)
{	   
	lcdSelectRegister(0x28);	//关闭显示
}   

/******************************************************************************
			函数说明：在指定区域内填充单个颜色
			入口参数：(sx,sy),(ex,ey)		填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
							color								画圆颜色
			返回参数：无
******************************************************************************/ 
void LCD_Fill(unsigned short sx, unsigned short sy, unsigned short ex, unsigned short ey, unsigned short color)
{          
	unsigned short i, j;
	unsigned short xlen = 0;
	unsigned short ylen = 0;
	
	xlen = ex - sx + 1;
	ylen = ey - sy + 1;
		
	lcdSetAddress(sx, sy, ex, ey);
    for(i = 0; i < xlen; i++)
	{
		for(j = 0; j < ylen; j++)
		{
			lcdWriteDataU16(color);
		}
	}		 
}  
/******************************************************************************
			函数说明：画线
			入口参数：x1,y1					起点坐标
							 x2,y2					终点坐标
							color						画圆颜色
			返回参数：无
******************************************************************************/ 
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color)
{
	unsigned short t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1; //计算坐标增量 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	
	//设置单步方向 
	if( delta_x > 0 )
	{
		incx=1; 
	}
	else if( delta_x == 0 )//垂直线 
	{
		incx=0;
	}
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}

	
	if( delta_y > 0 )
	{
		incy=1; 
	}
	else if( delta_y == 0 )//水平线 
	{
		incy=0;
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 
	
	if( delta_x > delta_y )//选取基本增量坐标轴 
	{
		distance = delta_x; 
	}
	else
	{
		distance=delta_y; 
	}
	
	for(t = 0; t <= distance + 1; t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow, uCol, color);//画点 
		xerr += delta_x ; 
		yerr += delta_y ; 
		if( xerr > distance ) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		
		if( yerr > distance ) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}  
}  

void LCD_DrawRectangle(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color)
{
	LCD_DrawLine(x1, y1, x2, y1, color);
	LCD_DrawLine(x1, y1, x1, y2, color);
	LCD_DrawLine(x1, y2, x2, y2, color);
	LCD_DrawLine(x2, y1, x2, y2, color);
}
/******************************************************************************
			函数说明：在指定位置画一个指定大小的圆
			入口参数：x,y						中心点
								r							半径
							color						画圆颜色
			返回参数：无
******************************************************************************/
void LCD_Draw_Circle(unsigned short x0, unsigned short y0, unsigned char r, unsigned short color)
{
	int a, b;
	int di;
	a = 0;
	b = r;	  
	di = 3 - ( r<<1 );             //判断下个点位置的标志
	while( a <= b )
	{
		LCD_DrawPoint(x0+a, y0-b, color);
 		LCD_DrawPoint(x0+b, y0-a, color);        
		LCD_DrawPoint(x0+b, y0+a, color);       
		LCD_DrawPoint(x0+a, y0+b, color);
		LCD_DrawPoint(x0-a, y0+b, color);
 		LCD_DrawPoint(x0-b, y0+a, color);
		LCD_DrawPoint(x0-a, y0-b, color);
  		LCD_DrawPoint(x0-b, y0-a, color);
		a++;
		//使用Bresenham算法画圆     
		if( di < 0 )
		{
			di += 4 * a + 6;	 
		} 
		else
		{
			di += 10 + 4 * ( a - b );   
			b--;
		} 						    
	}
} 	

/******************************************************************************
			函数说明：在指定位置画一个指定大小的圆
			入口参数：x,y						中心点
								r							半径
			返回参数：无
******************************************************************************/
void LCD_DrawFullCircle(unsigned short Xpos, unsigned short Ypos, unsigned short Radius, unsigned short Color)
{
	uint16_t x, y, r = Radius;
	for(y = Ypos - r; y < Ypos + r; y++)
	{
		for(x = Xpos - r;x < Xpos + r; x++)
		{
			if(((x - Xpos) * (x - Xpos) + (y - Ypos) * (y - Ypos)) <= r * r)
			{
				LCD_DrawPoint(x, y, Color);
			}
		}
	}
}


/******************************************************************************
			函数说明：在指定位置显示一个字符
			入口参数：x,y						起点坐标  
								size					字体大小
								num						要显示的字符:" "--->"~"
								fc						字体颜色
								bc						背景颜色
								mode					叠加方式(1)还是非叠加方式(0)
			返回参数：无
******************************************************************************/
void LCD_ShowChar(unsigned short x, unsigned short y, unsigned char num, unsigned char size, unsigned char mode, unsigned short pen_color, unsigned short back_color)
{  							  
  unsigned char temp, t1, t;
	unsigned short y0 = y;
	unsigned char csize = ( size / 8 + ( (size % 8) ? 1 : 0)) * (size / 2); //得到字体一个字符对应点阵集所占的字节数	
 	num = num - ' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	
	for(t = 0; t < csize; t++)
	{   
		if(size == 12)//调用1206字体
		{
			temp = asc2_1206[num][t]; 
		}
		else if(size == 16)//调用1608字体
		{
			temp=asc2_1608[num][t];	
		}
		else if(size == 24)	//调用2412字体
		{
			temp=asc2_2412[num][t];
		}
		else 
			return; //没有的字库
		
		for(t1 = 0; t1 < 8; t1++)
		{			    
			if( temp & 0x80 )
			{
				LCD_DrawPoint(x, y, pen_color);
			}
			else if( mode == 0)
			{
				LCD_DrawPoint(x, y, back_color);
			}
			temp <<= 1;
			y++;
			
			if(y >= 240)//超区域
			{
				return;	
			}	
			
			if((y-y0) == size)
			{
				y = y0;
				x++;
				if(x>=320)//超区域
				{
					return;	
				}
				break;
			}
		}  	 
	}  	    	   	 	  
}


/******************************************************************************
			函数说明：显示字符串
			入口参数：x,y						起点坐标
								width,height  区域大小  
								size					字体大小
								*p						字符串起始地址
								fc						字体颜色
								bc						背景颜色
			返回参数：无
******************************************************************************/
void LCD_ShowString(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char size, unsigned char *p, unsigned short pen_color, unsigned short back_color)
{         
	unsigned char x0 = x;
	width += x;
	height += y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x >= width)
		{
			x = x0;
			y += size;
		}
		
        if(y >= height)//退出
		{
			break;
		}
		
        LCD_ShowChar(x, y, *p, size, 0, pen_color, back_color);
        x += size / 2;
        p++;
    }  
}



/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y					显示坐标
                num 				要显示整数变量
                len 				要显示的位数
                fc 					字的颜色
                bc 					字的背景色
                sizey 			字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex, y, ' ', sizey, 0, fc, bc);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex, y, temp+48, sizey, 0, fc, bc);
	}
} 


/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y							显示坐标
                num 						要显示小数变量
                len 						要显示的位数
                fc 							字的颜色
                bc 							字的背景色
                sizey 					字号
      返回值：  无
******************************************************************************/
void LCD_ShowFloatNum(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.', sizey, 0, fc,bc);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48, sizey, 0,fc,bc);
	}
}



/******************************************************************************
      函数说明：显示十进制带符号
      入口数据：x,y							显示坐标
                num 						要显示10进制有符号变量
                fc 							字的颜色
                bc 							字的背景色
                sizey 					字号
      返回值：  无
******************************************************************************/
void LCD_ShowSignedNum(u16 x, u16 y, int32_t num, u8 len, u16 fc, u16 bc, u8 sizey)
{
	unsigned char number_Array[6] = {0};
	if(num < 0)
	{
		num = 0-num;
		LCD_ShowChar(x, y, '-', sizey, 0, fc, bc);
	}
	else
	{
		LCD_ShowChar(x, y, '+', sizey, 0, fc, bc);
	}
	if(len > 5)
	{
		len = 5;
	}
	
	
	number_Array[0] = (num / 10000) % 10 + 0x30;
	number_Array[1] = (num / 1000 ) % 10 + 0x30;
	number_Array[2] = (num / 100  ) % 10 + 0x30;
	number_Array[3] = (num / 10   ) % 10 + 0x30;
	number_Array[4] = (num / 1    ) % 10 + 0x30;
	number_Array[5] = '\0';
	LCD_ShowString(x+8, y, 10, 30, sizey, &number_Array[5-len], fc, bc);
	
}
	



/******************************************************************************
      函数说明：显示16进制变量
      入口数据：x,y						显示坐标
                num 					要显示16进制变量
                fc 						字的颜色
                bc 						字的背景色
                sizey 				字号
      返回值：  无
******************************************************************************/
void LCD_ShowHexNum(u16 x, u16 y, u32 num, u8 len, u16 fc, u16 bc,u8 sizey)
{
	uint8_t size;
	static const uint8_t hex[] = "0123456789ABCDEF";
	uint8_t res[4] = {0};
	uint8_t cnt = len, i = 0;
	size = sizey / 2;
	while(num != 0)
	{
		res[--cnt] = hex[num%16];
		num /= 16;
	}
	LCD_ShowChar(x, y, '0', sizey, 0, fc, bc);
	LCD_ShowChar(x + size, y, 'X', sizey, 0, fc, bc);
	for(i = 0; i < len; i++)
	{
		if(!res[i])
			res[i] = '0';
		LCD_ShowChar(x + sizey + size * i, y, res[i], sizey, 0, fc, bc);
	}
	
}





/******************************************************************************
      函数说明：程序汉字显示
      入口数据：x,y						要显示汉字的坐标
                c[2] 					汉字的数据 一个汉字两个字节表示
                fc 						字的颜色
                bc 						字的背景色
                sizey 				字号
      返回值：  无
******************************************************************************/
void GUI_sprintf_hz1616(unsigned short x, unsigned short y, unsigned char c[2], unsigned short pen_color, unsigned short back_color)
{
	unsigned char i, j, k, m; //定义临时变量
	unsigned short x0, y0;
	x0 = x;
	y0=y;	
	lcdSetAddress(x, y, x + 16 - 1, y + 16 - 1); //选择坐标位置	

	for (k = 0; k < 64; k++) //64表示自建汉字库中的个数，如超过，可以扩增
	{                        
		if ((code_GB_16[k].Index[0]==c[0]) && (code_GB_16[k].Index[1]==c[1])) //寻找对应汉字	 一个汉字需要两个字节
		{ 
			for(i = 0; i < 32; i++) //32个字节 每个字节都要一个点一个点处理 所以是处理了32X8次
			{								    
				m = code_GB_16[k].Msk[i];							//读取对应字节数据
				for(j = 0; j < 8; j++) 									//显示一个字节  一个字节8位 也就是8个点
				{
					//判断是否是要写入点
					if( (m&0x80) == 0x80)
					{
						LCD_Fast_DrawPoint(x, y, pen_color); // 如果是 给字体颜色
					}						 	 	
					else 			
					{
						LCD_Fast_DrawPoint(x, y, back_color); //如果不是 为背景色  给颜色
					}						
					m <<= 1; //左移一位  判断下一点
					x++;
					
					if(x - x0 == 16)
					{
						x = x0;
						y++;
						if(y - y0 >= 16)
						{
							return;
						}
					}
				} 
			}
		}  
	}	
}

/******************************************************************************
      函数说明：显示16x16汉字 
      入口数据：x,y						要显示汉字的坐标
                *str 					汉字的数据 一个汉字两个字节表示
                fc 						字的颜色
                bc 						字的背景色
      返回值：  无
******************************************************************************/ 
void GUI_sprintf_hz16x(unsigned short x1, unsigned short y1, unsigned char *str, unsigned short pen_color, unsigned short back_color)	  
{  
	unsigned char l = 0;
	while(*str)
	{	
		if(*str>=0x80)
		{
			GUI_sprintf_hz1616(x1 + l * 8, y1, (unsigned char*)str, pen_color, back_color);
			str += 2;
			l += 2;
		}
	}	
}

/******************************************************************************
      函数说明：显示32x32单个汉字 
      入口数据： x,y						要显示汉字的坐标
                c[2] 			  	汉字的数据 一个汉字两个字节表示
                fc 						字的颜色
                bc 						字的背景色
      返回值：  无
******************************************************************************/
void GUI_sprintf_hz3232(unsigned short x, unsigned short y, unsigned char c[2], unsigned short pen_color, unsigned short back_color)
{
	unsigned char i, j, k;
	unsigned short x0, y0;
	x0 = x;
	y0 = y;	

	lcdSetAddress(x, y, x + 32 - 1, y + 32 - 1); //选择坐标位置	
	
	for (k = 0; k < 100; k++) //自建汉字库（k最大值和汉字库中汉字个数有关），循环查询内码
	{ 
		if ((code_GB_32[k].Index[0]==c[0])&&(code_GB_32[k].Index[1]==c[1])) //寻找要显示的汉字
		{ 
			for(i = 0; i < 128; i++) //1个汉字要128个字节点来显示
			{
				unsigned char m = code_GB_32[k].Msk[i]; //一个字节一个字节的显示
				for(j = 0; j < 8; j++) 
				{
					//判断是否是要写入点
					if((m&0x80)==0x80) // 如果是,给字体颜色
					{
						LCD_Fast_DrawPoint(x,y,pen_color);		
					}
					else //如果不是,为背景色
					{
						LCD_Fast_DrawPoint(x,y,back_color);	
					}
					m<<=1;
					x++;
					if(x - x0 == 32)
					{
						x = x0;
						y++;
						if(y - y0 >= 32)
						{
							return;
						}
					}
				}    
			}
		}  
	}	
}

/******************************************************************************
      函数说明：显示32x32,32x16i字母串 
      入口数据： x,y						要显示汉字的坐标
                *str 			  	汉字的数据 一个汉字两个字节表示
                fc 						字的颜色
                bc 						字的背景色
      返回值：  无
******************************************************************************/
void GUI_sprintf_hz32x(unsigned short x1, unsigned short y1, unsigned char *str, unsigned short pen_color, unsigned short back_color)	 
{  
	unsigned char l = 0;	  
	while(*str)
	{
		if(*str>=0x80)		 	        
		{
			GUI_sprintf_hz3232(x1 + l * 8, y1, (unsigned char*)str, pen_color, back_color);
			l += 4;
			str += 2;
		}
	}
}




/******************************************************************************
      函数说明：画图 
      入口数据： *p 传入图片地址
      返回值：  无
******************************************************************************/
void Lcd_ShowPature(const unsigned char *p)
{
	unsigned int i, j;
	uint32_t k = 0;
	lcdClear(WHITE);//清屏  
	
	lcdSetAddress(0, 0, 127, 127);
	
	
	for(i=0; i<128; i++)
	{
		for(j=0; j<128; j++)
		{
				lcdWriteDataU8(p[k*2]);
				lcdWriteDataU8(p[k*2+1]);
				k++;
		}
	}
	
}
	

