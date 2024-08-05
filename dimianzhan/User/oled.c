

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



#include "oledfont.h"
#include "oled.h"
#include "stm32f1xx_hal.h"

extern I2C_HandleTypeDef hi2c1; // 使用外部声明的I2C句柄






extern u8 ok_flag; // 声明外部变量ok_flag











#define OLED_ADD 0x78 // OLED的I2C地址

static void oled_cmd(uint8_t i2c_cmd)
{
    uint8_t cmd[1];
    cmd[0] = i2c_cmd;
    HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, 0x00, I2C_MEMADD_SIZE_8BIT, cmd, 1, 100);
}

static void oled_data(uint8_t i2c_data)
{
    uint8_t data[1];
    data[0] = i2c_data;
    HAL_I2C_Mem_Write(&hi2c1, OLED_ADD, 0x40, I2C_MEMADD_SIZE_8BIT, data, 1, 100);
}

static void oled_origin(uint8_t x, uint8_t y) 
{
    oled_cmd(0xb0 + y);
    oled_cmd(((x & 0xf0) >> 4) | 0x10);
    oled_cmd((x & 0x0f) | 0x01);
}

void oled_clear(void)
{
    uint8_t i, n;            
    for(i = 0; i < 8; i++)  
    {  
        oled_cmd(0xb0 + i);
        oled_cmd(0x00); 
        oled_cmd(0x10); 
        for(n = 0; n < 128; n++)
            oled_data(0);
    } 
}

void oled_full(uint8_t data) //0xff
{
    uint8_t m, n;
    for(m = 0; m < 8; m++)
    {
        oled_cmd(0xb0 + m); // page0-page7
        oled_cmd(0x00); // low column start address
        oled_cmd(0x10); // high column start address
        for(n = 0; n < 128; n++)
        {
            oled_data(data);
        }
    }
}

void oled_init(void)
{
    HAL_Delay(100);
    
    oled_cmd(0xAE); // display off
    oled_cmd(0x20); // Set Memory Addressing Mode
    oled_cmd(0x10); // Page Addressing Mode (RESET)
    oled_cmd(0xb0); // Set Page Start Address for Page Addressing Mode, 0-7
    oled_cmd(0xc8); // Set COM Output Scan Direction
    oled_cmd(0x00); // set low column address
    oled_cmd(0x10); // set high column address
    oled_cmd(0x40); // set start line address
    oled_cmd(0x81); // set contrast control register
    oled_cmd(0xff); // max contrast
    oled_cmd(0xa1); // set segment re-map 0 to 127
    oled_cmd(0xa6); // set normal display
    oled_cmd(0xa8); // set multiplex ratio(1 to 64)
    oled_cmd(0x3F); // 
    oled_cmd(0xa4); // Output follows RAM content
    oled_cmd(0xd3); // set display offset
    oled_cmd(0x00); // no offset
    oled_cmd(0xd5); // set display clock divide ratio/oscillator frequency
    oled_cmd(0xf0); // set divide ratio
    oled_cmd(0xd9); // set pre-charge period
    oled_cmd(0x22); // 
    oled_cmd(0xda); // set com pins hardware configuration
    oled_cmd(0x12);
    oled_cmd(0xdb); // set vcomh
    oled_cmd(0x20); // 0.77xVcc
    oled_cmd(0x8d); // set DC-DC enable
    oled_cmd(0x14); // 
    oled_cmd(0xaf); // turn on oled panel
    
    oled_full(0x00);
}

void oled_display_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; // 得到偏移后的值
    if(x > 128 - 1) { x = 0; y = y + 2; }
    if(Char_Size == 16)
    {
        oled_origin(x, y);    
        for(i = 0; i < 8; i++)
            oled_data(F8X16[c*16 + i]);
        oled_origin(x, y + 1);
        for(i = 0; i < 8; i++)
            oled_data(F8X16[c*16 + i + 8]);
    }
    else
    {    
        oled_origin(x, y);
        for(i = 0; i < 6; i++)
            oled_data(F6x8[c][i]);
    }
}








// 新增的函数：显示全局变量 ok_flag 的值
void oled_display_ok_flag(uint8_t x, uint8_t y)
{
    char buffer[20];
    sprintf(buffer, "ok_flag: %d", ok_flag); // 将ok_flag的值转换为字符串
    oled_show_string(x, y, buffer, 2); // 在(x, y)位置显示ok_flag的值，字符大小为2
}






















void oled_show_string(uint8_t x, uint8_t y, char ch[], uint8_t TextSize)
{
    uint8_t c = 0, i = 0, j = 0;
    switch(TextSize)
    {
        case 1:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 126)
                {
                    x = 0;
                    y++;
                }
                oled_origin(x, y);
                for(i = 0; i < 6; i++)
                    oled_data(F6x8[c][i]);
                x += 6;
                j++;
            }
        }
        break;
        case 2:
        {
            while(ch[j] != '\0')
            {
                c = ch[j] - 32;
                if(x > 120)
                {
                    x = 0;
                    y++;
                }
                oled_origin(x, y);
                for(i = 0; i < 8; i++)
                    oled_data(F8X16[c*16 + i]);
                oled_origin(x, y + 1);
                for(i = 0; i < 8; i++)
                    oled_data(F8X16[c*16 + i + 8]);
                x += 8;
                j++;
            }
        }
        break;
    }
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




























