

/**
  ******************************************************************************
  * 文件名程: oled.c
  * 作    者: By Sw Young 
  * 版    本: V1.0
  * 功    能:
  * 编写日期: 2018.5.5
  ******************************************************************************
  * 说明：
  * 硬件平台：STM32F103
  *   *****
  * 软件设计说明：
  *   *****
  * Github：
  ******************************************************************************
**/
#include "oled.h"

#include "oledfont.h"
#include "OLED_bmp.h"



//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127
//[1]0 1 2 3 ... 127
//[2]0 1 2 3 ... 127
//[3]0 1 2 3 ... 127
//[4]0 1 2 3 ... 127
//[5]0 1 2 3 ... 127
//[6]0 1 2 3 ... 127
//[7]0 1 2 3 ... 127



void OLED_GPIO_Init(void)
{
	  GPIO_InitTypeDef OLED_GPIO_Initure;

    __HAL_RCC_GPIOB_CLK_ENABLE();           	//开启GPIOG时钟
	 __HAL_RCC_GPIOD_CLK_ENABLE();           	//开启GPIOD时钟

	
    OLED_GPIO_Initure.Pin=GPIO_PIN_2;        //SDA
    OLED_GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  	//推挽输出
    OLED_GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;    //高速
	
    HAL_GPIO_Init(GPIOD,&OLED_GPIO_Initure);
	
	 OLED_GPIO_Initure.Pin=GPIO_PIN_3;        //SCL
	HAL_GPIO_Init(GPIOB,&OLED_GPIO_Initure);
}

/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{

    OLED_SCLK_Set();
    OLED_SDIN_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
OLED_SCLK_Set() ;
//  OLED_SCLK_Clr();
    OLED_SDIN_Clr();
    OLED_SDIN_Set();

}

void IIC_Wait_Ack()
{
    OLED_SCLK_Set() ;
    OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte)
{
    unsigned char i;
    unsigned char m,da;
    da=IIC_Byte;
    OLED_SCLK_Clr();
    for(i=0;i<8;i++)
    {
            m=da;
        //  OLED_SCLK_Clr();
        m=m&0x80;
        if(m==0x80)
        {OLED_SDIN_Set();}
        else OLED_SDIN_Clr();
            da=da<<1;
        OLED_SCLK_Set();
        OLED_SCLK_Clr();
        }
}

/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //Slave address,SA0=0
    IIC_Wait_Ack();
   Write_IIC_Byte(0x00);            //write command
    IIC_Wait_Ack();
   Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ack();
   IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data)
{
   IIC_Start();
   Write_IIC_Byte(0x78);            //D/C#=0; R/W#=0
    IIC_Wait_Ack();
   Write_IIC_Byte(0x40);            //write data
    IIC_Wait_Ack();
   Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();
   IIC_Stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
    if(cmd)
            {

   Write_IIC_Data(dat);

        }
    else {
   Write_IIC_Command(dat);

    }


}

/********************************************
// fill_Picture
********************************************/
void fill_picture(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        OLED_WR_Byte(0xb0+m,0);     //page0-page1
        OLED_WR_Byte(0x00,0);       //low column start address
        OLED_WR_Byte(0x10,0);       //high column start address
        for(n=0;n<128;n++)
            {
                OLED_WR_Byte(fill_Data,1);
            }
    }
}

//坐标设置

    void OLED_Set_Pos(unsigned char x, unsigned char y)
{   OLED_WR_Byte(0xb0+y,OLED_CMD);
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD);
}
//开启OLED显示
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
    OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
        OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA);
    } //更新显示
}
void OLED_On(void)
{
    u8 i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
        OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
        OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址
        for(n=0;n<128;n++)OLED_WR_Byte(1,OLED_DATA);
    } //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{
    unsigned char c=0,i=0;
        c=chr-' ';//得到偏移后的值
        if(x>Max_Column-1){x=0;y=y+2;}
        if(Char_Size ==16)
            {
            OLED_Set_Pos(x,y);
            for(i=0;i<8;i++)
            OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
            OLED_Set_Pos(x,y+1);
            for(i=0;i<8;i++)
            OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
            }
            else {
                OLED_Set_Pos(x,y);
                for(i=0;i<6;i++)
                OLED_WR_Byte(F6x8[c][i],OLED_DATA);

            }
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
    u32 result=1;
    while(n--)result*=m;
    return result;
}
//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//num:数值(0~4294967295);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{
    u8 t,temp;
    u8 enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
                continue;
            }else enshow=1;

        }
        OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
    }
}



//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {       OLED_ShowChar(x,y,chr[j],Char_Size);
            x+=8;
        if(x>120){x=0;y+=2;}
            j++;
    }
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{
    u8 t,adder=0;
    OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
        {
                OLED_WR_Byte(Hzk[2*no][t],OLED_DATA);
                adder+=1;
     }
        OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
            {
                OLED_WR_Byte(Hzk[2*no+1][t],OLED_DATA);
                adder+=1;
      }
}

void OLED_ShowCHinese1(u8 x,u8 y,u8 no)
{
    u8 t,adder=0;
    OLED_Set_Pos(x,y);
    for(t=0;t<16;t++)
        {
                OLED_WR_Byte(Hzk1[2*no][t],OLED_DATA);
                adder+=1;
     }
        OLED_Set_Pos(x,y+1);
    for(t=0;t<16;t++)
            {
                OLED_WR_Byte(Hzk1[2*no+1][t],OLED_DATA);
                adder+=1;
      }
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{
 unsigned int j=0;
 unsigned char x,y;

  if(y1%8==0) y=y1/8;
  else y=y1/8+1;
    for(y=y0;y<y1;y++)
    {
        OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
        {
            OLED_WR_Byte(BMP[j++],OLED_DATA);
        }
    }
}

//初始化SSD1306
void OLED_Init(void)
{

    OLED_GPIO_Init();

    OLED_WR_Byte(0xAE,OLED_CMD);//--display off
    OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
    OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
    OLED_WR_Byte(0x40,OLED_CMD);//--set start line address
    OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
    OLED_WR_Byte(0x81,OLED_CMD); // contract control
    OLED_WR_Byte(0xFF,OLED_CMD);//--128
    OLED_WR_Byte(0xA1,OLED_CMD);//set segment remap
    OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
    OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
    OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty
    OLED_WR_Byte(0xC8,OLED_CMD);//Com scan direction
    OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
    OLED_WR_Byte(0x00,OLED_CMD);//

    OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
    OLED_WR_Byte(0x80,OLED_CMD);//

    OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
    OLED_WR_Byte(0x05,OLED_CMD);//

    OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
    OLED_WR_Byte(0xF1,OLED_CMD);//

    OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
    OLED_WR_Byte(0x12,OLED_CMD);//

    OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
    OLED_WR_Byte(0x30,OLED_CMD);//

    OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable
    OLED_WR_Byte(0x14,OLED_CMD);//

    OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel
}





//下面是待修改的代码
//显示界面
//void show_UI(void)
//{
//	
//	//滚动显示所有界面
////	show_first_interface();
////	delay_ms(500);
////	show_menu();
////	delay_ms(500);
////	show_chosen_current();
////	delay_ms(500);
////	show_current_test();
////	delay_ms(500);
////	show_chosen_rotation();
////	delay_ms(500);
////	show_rotation_test();
////	delay_ms(500);
////	show_chosen_all();
////	delay_ms(500);
////	show_all_test();
////	delay_ms(500);

//	if(time13 >= 500)
//	{
//		time13 = 0;
//		INTERFACE_Statment();//状态机显示菜单
//	}
//	
//}


//void show_chosen_current(void)
//{
//	unsigned char table[20]  = ">>";
//	unsigned char table1[10]  = "  ";

//	OLED_ShowStr(0, 0, table, 2);    //>>
//	OLED_ShowStr(0, 2, table1, 2);    //
//	OLED_ShowStr(0, 4, table1, 2);    //
//}


//void show_chosen_rotation(void)
//{

//	unsigned char table[20]  = ">>";
//	unsigned char table1[10]  = "  ";
//	
//	OLED_ShowStr(0, 0, table1, 2);    //>>
//	OLED_ShowStr(0, 2, table, 2);    //>>
//	OLED_ShowStr(0, 4, table1, 2);    //>>
//	
//}


//void show_chosen_all(void)
//{

//	unsigned char table[20]  = ">>";
//	unsigned char table1[10]  = "  ";

//	
//	OLED_ShowStr(0, 0, table1, 2);    //>>
//	OLED_ShowStr(0, 2, table1, 2);    //>>
//	OLED_ShowStr(0, 4, table, 2);    //>>
//		
//}


//void show_menu(void)
//{

//	unsigned char table0[20] = "1.";
//	unsigned char table1[20] = "2.";
//	unsigned char table2[20] = "3.";
//		
//	OLED_ShowStr(16, 0, table0, 2);  //1.
//	OLED_ShowCN(32, 0, 5);			//电
//	OLED_ShowCN(48, 0, 7);			//流
//	OLED_ShowCN(64, 0, 9);			//检
//	OLED_ShowCN(80, 0, 10);			//测
//	
//	
//	OLED_ShowStr(16, 2, table1, 2);  //2.
//	OLED_ShowCN(32, 2, 3);			//转
//	OLED_ShowCN(48, 2, 4);			//速
//	OLED_ShowCN(64, 2, 9);			//检
//	OLED_ShowCN(80, 2, 10);			//测
//	
//	
//	OLED_ShowStr(16, 4, table2, 2);  //3.
//	OLED_ShowCN(32, 4, 15);			//一
//	OLED_ShowCN(48, 4, 16);			//键
//	OLED_ShowCN(64, 4, 9);			//检
//	OLED_ShowCN(80, 4, 10);			//测
//	
//	
//	OLED_ShowCN(0, 6, 11);			//确
//	OLED_ShowCN(16, 6, 12);			//认
//	
//	OLED_ShowCN(96, 6, 13);			//返
//	OLED_ShowCN(112, 6, 14);		//回
//}


//void show_current_test(void)
//{

//	//显示经取模软件设定好的汉字
//	unsigned char table0[20] = ":xx  ";
//	unsigned char table1[20] = ":xx  ";
//	unsigned char table5[20] = ":       ";
//	
//	
//	sprintf((char *)table0,":%d mA", 123);
//	sprintf((char *)table1,":%d mA", 456);
//	

//	OLED_ShowCN(32, 0, 5);			//电
//	OLED_ShowCN(48, 0, 7);			//流
//	OLED_ShowCN(64, 0, 9);			//检
//	OLED_ShowCN(80, 0, 10);			//测
//	
//	
//	OLED_ShowCN(0, 2, 0);		     //左
//	OLED_ShowCN(16, 2, 2);			 //侧
//	OLED_ShowCN(32, 2, 5);		     //电
//	OLED_ShowCN(48, 2, 7);			 //流
//	
//	
//	OLED_ShowCN(0, 4, 1);		     //右
//	OLED_ShowCN(16, 4, 2);			 //侧
//	OLED_ShowCN(32, 4, 5);		     //电
//	OLED_ShowCN(48, 4, 7);			 //流
//	
//	
//	OLED_ShowCN(0, 6, 9);			//检
//	OLED_ShowCN(16, 6, 10);			//测
//	
//	
//	OLED_ShowCN(96, 6, 13);			//返
//	OLED_ShowCN(112, 6, 14);		//回
//	
//	if(time >= 500)
//	{
//		time = 0;
//		//清数据
//		OLED_ShowStr(64, 2, table5, 2); 
//		OLED_ShowStr(64, 4, table5, 2);
//		OLED_ShowStr(64, 2, table0, 2);  //显示对应的数值
//		OLED_ShowStr(64, 4, table1, 2);  //显示对应的数值
//	}
//}

//void show_rotation_test(void)
//{
//	//显示经取模软件设定好的汉字
//	unsigned char table0[20] = ":xx  ";
//	unsigned char table1[20] = ":xx  ";
//	unsigned char table5[20] = ":       ";
//	
////	if((rotation_left_value >= 999)||(rotation_right_value >= 999))
////	{
////		rotation_left_value = 0;
////		rotation_right_value = 0;
////	}
//	
////	sprintf((char *)table0,":%d rpm", (int)rotation_left_value);
////	sprintf((char *)table1,":%d rpm", (int)rotation_right_value);
//	
//	
//	OLED_ShowCN(32, 0, 3);			//转
//	OLED_ShowCN(48, 0, 4);			//速
//	OLED_ShowCN(64, 0, 9);			//检
//	OLED_ShowCN(80, 0, 10);			//测
//	
//	
//	OLED_ShowCN(0, 2, 0);		     //左
//	OLED_ShowCN(16, 2, 2);			 //侧
//	OLED_ShowCN(32, 2, 3);		     //转
//	OLED_ShowCN(48, 2, 4);			 //速

//	
//	OLED_ShowCN(0, 4, 1);		     //右
//	OLED_ShowCN(16, 4, 2);			 //侧
//	OLED_ShowCN(32, 4, 3);		     //转
//	OLED_ShowCN(48, 4, 4);			 //速
//	
//	
//	OLED_ShowCN(0, 6, 9);			//检
//	OLED_ShowCN(16, 6, 10);			//测
//	
//	
//	OLED_ShowCN(96, 6, 13);			//返
//	OLED_ShowCN(112, 6, 14);		//回
//	
//	if(time >= 500)
//	{
//		time = 0;
//		//清数据
//		OLED_ShowStr(64, 2, table5, 2); 
//		OLED_ShowStr(64, 4, table5, 2);
//		
//		OLED_ShowStr(64, 2, table0, 2);  //显示对应的数值
//		OLED_ShowStr(64, 4, table1, 2);  //显示对应的数值
//	}
//}

//void show_all_test(void)
//{


//	//显示经取模软件设定好的汉字
//	unsigned char table[20]  = ":xx  ";
//	unsigned char table0[20] = ":xx  ";
//	unsigned char table1[20] = ":xx  ";
//	unsigned char table2[20] = ":xx  ";
//	
//	
//		
//		OLED_ShowCN(0, 0, 3);//转
//		OLED_ShowCN(16, 0, 6);//差
////		OLED_ShowCN(97, 0, 3);//转
////		OLED_ShowCN(113, 0, 8);//向
//				
//		OLED_ShowCN(0, 2, 0);		     //左
//		OLED_ShowCN(16, 2, 2);			 //侧
////		OLED_ShowCN(32, 4, 3);		     //转
////		OLED_ShowCN(48, 4, 4);			 //速

//		OLED_ShowCN(0, 4, 1);		     //右
//		OLED_ShowCN(16, 4, 2);			 //侧
////		OLED_ShowCN(32, 2, 3);		     //转
////		OLED_ShowCN(48, 2, 4);			 //速
//		
//		OLED_ShowCN(0, 6, 5);           //电
//		OLED_ShowCN(16, 6, 7);			 //流
//		
//		OLED_ShowCN(96, 6, 13);			//返
//		OLED_ShowCN(112, 6, 14);		//回
//		
//		
//	if(time >= 500)
//	{
//		time = 0;

//		//清数据

//		
//		OLED_ShowStr(32,0,table,2);//
//		OLED_ShowStr(32, 2, table2, 2);//
//		OLED_ShowStr(32, 4, table0, 2);// 
//		OLED_ShowStr(32, 6, table1, 2);//

//	}
//}

//void show_first_interface(void)
//{
//	show_bmp(1);
//}

//void show_bmp(u8 a)
//{
//	switch(a)
//	{
//		case 1:
//				OLED_DrawBMP(0,0,128,8,BMP1);
//			break;
//			
////		case 2:
////				OLED_DrawBMP(0,0,128,8,BMP2);
////			break;
//			
//			
//		default:
//			break;
//			
//	}
//	
//}




























