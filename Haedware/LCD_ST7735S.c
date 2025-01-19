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

//LCD����
void LCD_DrawPoint(u16 x, u16 y, u16 color)
{
	 lcdSetAddress(x, y, x, y);//���ù��λ�� 
	 lcdWriteDataU16(color);
}


/******************************************************************************
			����˵�������ٻ���
			��ڲ�����x,y					�������
							 color				��Բ��ɫ
			���ز�������
******************************************************************************/ 
void LCD_Fast_DrawPoint(u16 x, u16 y, u16 color)
{
	lcdWriteDataU16(color);
}	

//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	lcdSelectRegister(0x29);	//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	lcdSelectRegister(0x28);	//�ر���ʾ
}   

/******************************************************************************
			����˵������ָ����������䵥����ɫ
			��ڲ�����(sx,sy),(ex,ey)		�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
							color								��Բ��ɫ
			���ز�������
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
			����˵��������
			��ڲ�����x1,y1					�������
							 x2,y2					�յ�����
							color						��Բ��ɫ
			���ز�������
******************************************************************************/ 
void LCD_DrawLine(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color)
{
	unsigned short t; 
	int xerr = 0, yerr = 0, delta_x, delta_y, distance; 
	int incx, incy, uRow, uCol; 
	delta_x = x2 - x1; //������������ 
	delta_y = y2 - y1; 
	uRow = x1; 
	uCol = y1; 
	
	//���õ������� 
	if( delta_x > 0 )
	{
		incx=1; 
	}
	else if( delta_x == 0 )//��ֱ�� 
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
	else if( delta_y == 0 )//ˮƽ�� 
	{
		incy=0;
	}
	else
	{
		incy = -1;
		delta_y = -delta_y;
	} 
	
	if( delta_x > delta_y )//ѡȡ�������������� 
	{
		distance = delta_x; 
	}
	else
	{
		distance=delta_y; 
	}
	
	for(t = 0; t <= distance + 1; t++ )//������� 
	{  
		LCD_DrawPoint(uRow, uCol, color);//���� 
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
			����˵������ָ��λ�û�һ��ָ����С��Բ
			��ڲ�����x,y						���ĵ�
								r							�뾶
							color						��Բ��ɫ
			���ز�������
******************************************************************************/
void LCD_Draw_Circle(unsigned short x0, unsigned short y0, unsigned char r, unsigned short color)
{
	int a, b;
	int di;
	a = 0;
	b = r;	  
	di = 3 - ( r<<1 );             //�ж��¸���λ�õı�־
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
		//ʹ��Bresenham�㷨��Բ     
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
			����˵������ָ��λ�û�һ��ָ����С��Բ
			��ڲ�����x,y						���ĵ�
								r							�뾶
			���ز�������
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
			����˵������ָ��λ����ʾһ���ַ�
			��ڲ�����x,y						�������  
								size					�����С
								num						Ҫ��ʾ���ַ�:" "--->"~"
								fc						������ɫ
								bc						������ɫ
								mode					���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
			���ز�������
******************************************************************************/
void LCD_ShowChar(unsigned short x, unsigned short y, unsigned char num, unsigned char size, unsigned char mode, unsigned short pen_color, unsigned short back_color)
{  							  
  unsigned char temp, t1, t;
	unsigned short y0 = y;
	unsigned char csize = ( size / 8 + ( (size % 8) ? 1 : 0)) * (size / 2); //�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num = num - ' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	
	for(t = 0; t < csize; t++)
	{   
		if(size == 12)//����1206����
		{
			temp = asc2_1206[num][t]; 
		}
		else if(size == 16)//����1608����
		{
			temp=asc2_1608[num][t];	
		}
		else if(size == 24)	//����2412����
		{
			temp=asc2_2412[num][t];
		}
		else 
			return; //û�е��ֿ�
		
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
			
			if(y >= 240)//������
			{
				return;	
			}	
			
			if((y-y0) == size)
			{
				y = y0;
				x++;
				if(x>=320)//������
				{
					return;	
				}
				break;
			}
		}  	 
	}  	    	   	 	  
}


/******************************************************************************
			����˵������ʾ�ַ���
			��ڲ�����x,y						�������
								width,height  �����С  
								size					�����С
								*p						�ַ�����ʼ��ַ
								fc						������ɫ
								bc						������ɫ
			���ز�������
******************************************************************************/
void LCD_ShowString(unsigned short x, unsigned short y, unsigned short width, unsigned short height, unsigned char size, unsigned char *p, unsigned short pen_color, unsigned short back_color)
{         
	unsigned char x0 = x;
	width += x;
	height += y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x >= width)
		{
			x = x0;
			y += size;
		}
		
        if(y >= height)//�˳�
		{
			break;
		}
		
        LCD_ShowChar(x, y, *p, size, 0, pen_color, back_color);
        x += size / 2;
        p++;
    }  
}



/******************************************************************************
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      ����˵������ʾ��������
      ������ݣ�x,y					��ʾ����
                num 				Ҫ��ʾ��������
                len 				Ҫ��ʾ��λ��
                fc 					�ֵ���ɫ
                bc 					�ֵı���ɫ
                sizey 			�ֺ�
      ����ֵ��  ��
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
      ����˵������ʾ��λС������
      ������ݣ�x,y							��ʾ����
                num 						Ҫ��ʾС������
                len 						Ҫ��ʾ��λ��
                fc 							�ֵ���ɫ
                bc 							�ֵı���ɫ
                sizey 					�ֺ�
      ����ֵ��  ��
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
      ����˵������ʾʮ���ƴ�����
      ������ݣ�x,y							��ʾ����
                num 						Ҫ��ʾ10�����з��ű���
                fc 							�ֵ���ɫ
                bc 							�ֵı���ɫ
                sizey 					�ֺ�
      ����ֵ��  ��
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
      ����˵������ʾ16���Ʊ���
      ������ݣ�x,y						��ʾ����
                num 					Ҫ��ʾ16���Ʊ���
                fc 						�ֵ���ɫ
                bc 						�ֵı���ɫ
                sizey 				�ֺ�
      ����ֵ��  ��
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
      ����˵������������ʾ
      ������ݣ�x,y						Ҫ��ʾ���ֵ�����
                c[2] 					���ֵ����� һ�����������ֽڱ�ʾ
                fc 						�ֵ���ɫ
                bc 						�ֵı���ɫ
                sizey 				�ֺ�
      ����ֵ��  ��
******************************************************************************/
void GUI_sprintf_hz1616(unsigned short x, unsigned short y, unsigned char c[2], unsigned short pen_color, unsigned short back_color)
{
	unsigned char i, j, k, m; //������ʱ����
	unsigned short x0, y0;
	x0 = x;
	y0=y;	
	lcdSetAddress(x, y, x + 16 - 1, y + 16 - 1); //ѡ������λ��	

	for (k = 0; k < 64; k++) //64��ʾ�Խ����ֿ��еĸ������糬������������
	{                        
		if ((code_GB_16[k].Index[0]==c[0]) && (code_GB_16[k].Index[1]==c[1])) //Ѱ�Ҷ�Ӧ����	 һ��������Ҫ�����ֽ�
		{ 
			for(i = 0; i < 32; i++) //32���ֽ� ÿ���ֽڶ�Ҫһ����һ���㴦�� �����Ǵ�����32X8��
			{								    
				m = code_GB_16[k].Msk[i];							//��ȡ��Ӧ�ֽ�����
				for(j = 0; j < 8; j++) 									//��ʾһ���ֽ�  һ���ֽ�8λ Ҳ����8����
				{
					//�ж��Ƿ���Ҫд���
					if( (m&0x80) == 0x80)
					{
						LCD_Fast_DrawPoint(x, y, pen_color); // ����� ��������ɫ
					}						 	 	
					else 			
					{
						LCD_Fast_DrawPoint(x, y, back_color); //������� Ϊ����ɫ  ����ɫ
					}						
					m <<= 1; //����һλ  �ж���һ��
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
      ����˵������ʾ16x16���� 
      ������ݣ�x,y						Ҫ��ʾ���ֵ�����
                *str 					���ֵ����� һ�����������ֽڱ�ʾ
                fc 						�ֵ���ɫ
                bc 						�ֵı���ɫ
      ����ֵ��  ��
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
      ����˵������ʾ32x32�������� 
      ������ݣ� x,y						Ҫ��ʾ���ֵ�����
                c[2] 			  	���ֵ����� һ�����������ֽڱ�ʾ
                fc 						�ֵ���ɫ
                bc 						�ֵı���ɫ
      ����ֵ��  ��
******************************************************************************/
void GUI_sprintf_hz3232(unsigned short x, unsigned short y, unsigned char c[2], unsigned short pen_color, unsigned short back_color)
{
	unsigned char i, j, k;
	unsigned short x0, y0;
	x0 = x;
	y0 = y;	

	lcdSetAddress(x, y, x + 32 - 1, y + 32 - 1); //ѡ������λ��	
	
	for (k = 0; k < 100; k++) //�Խ����ֿ⣨k���ֵ�ͺ��ֿ��к��ָ����йأ���ѭ����ѯ����
	{ 
		if ((code_GB_32[k].Index[0]==c[0])&&(code_GB_32[k].Index[1]==c[1])) //Ѱ��Ҫ��ʾ�ĺ���
		{ 
			for(i = 0; i < 128; i++) //1������Ҫ128���ֽڵ�����ʾ
			{
				unsigned char m = code_GB_32[k].Msk[i]; //һ���ֽ�һ���ֽڵ���ʾ
				for(j = 0; j < 8; j++) 
				{
					//�ж��Ƿ���Ҫд���
					if((m&0x80)==0x80) // �����,��������ɫ
					{
						LCD_Fast_DrawPoint(x,y,pen_color);		
					}
					else //�������,Ϊ����ɫ
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
      ����˵������ʾ32x32,32x16i��ĸ�� 
      ������ݣ� x,y						Ҫ��ʾ���ֵ�����
                *str 			  	���ֵ����� һ�����������ֽڱ�ʾ
                fc 						�ֵ���ɫ
                bc 						�ֵı���ɫ
      ����ֵ��  ��
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
      ����˵������ͼ 
      ������ݣ� *p ����ͼƬ��ַ
      ����ֵ��  ��
******************************************************************************/
void Lcd_ShowPature(const unsigned char *p)
{
	unsigned int i, j;
	uint32_t k = 0;
	lcdClear(WHITE);//����  
	
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
	

