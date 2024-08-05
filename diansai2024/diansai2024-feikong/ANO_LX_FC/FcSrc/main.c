/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * 作者    ：匿名科创
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
 * 描述    ：主循环
**********************************************************************************/
#include "SysConfig.h"
#include "Ano_Scheduler.h"
//#include "ANO_LX.h"
#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
	while (1)
	{
		//当系统出错后，会进入这个死循环
	}
}
#endif
//=======================================================================================
//=======================================================================================
int main(void)
{
	//进行所有设备的初始化，并将初始化结果保存
	All_Init();
	//调度器初始化，系统为裸奔，这里人工做了一个时分调度器
	Scheduler_Setup();
	//TIM_SetCompare4(TIM4,15);
	/*case WKUP_PRES:	  //0
				  pwmval = 5;
			      break;
				case KEY0_PRES:	//45
				  pwmval = 10;
				  TIM_SetCompare1(TIM14,pwmval);
				  break;
				case KEY1_PRES:	//90 
				  pwmval = 15;
				  break;
				case KEY2_PRES:	//135
				  pwmval = 20;
*/
	while (1)
	{
		//运行任务调度器，所有系统功能，除了中断服务函数，都在任务调度器内完成
		Scheduler_Run();
	//	TIM_SetCompare4(TIM4,150);
	//	TIM_SetCompare4(TIM4,140); //140--220
	}
}
/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
