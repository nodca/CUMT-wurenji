/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * 作者    ：匿名科创
 * 官网    ：www.anotc.com
 * 淘宝    ：anotc.taobao.com
 * 技术Q群 ：190169595
 * 描述    ：定时器驱动
**********************************************************************************/

#include "Drv_Timer.h"
#include "Drv_led.h"

#define SYS_TIMx_IRQn TIM7_IRQn
#define SYS_TIMx TIM7
#define SYS_RCC_TIMx RCC_APB1Periph_TIM7

void TIM_CONF(u16 period_ms) //APB1  84M
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* 使能时钟 */
    RCC_APB1PeriphClockCmd(SYS_RCC_TIMx, ENABLE);

    TIM_DeInit(SYS_TIMx);

    /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period = period_ms;

    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 时钟预分频数为 */
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;

    /* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数

    TIM_TimeBaseInit(SYS_TIMx, &TIM_TimeBaseStructure);

    TIM_ClearFlag(SYS_TIMx, TIM_FLAG_Update);

    TIM_ITConfig(SYS_TIMx, TIM_IT_Update, ENABLE);

    TIM_Cmd(SYS_TIMx, ENABLE);

    RCC_APB1PeriphClockCmd(SYS_RCC_TIMx, DISABLE); /*先关闭等待使用*/
}
void TIM_NVIC()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    //    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    NVIC_InitStructure.NVIC_IRQChannel = SYS_TIMx_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = NVIC_TIME_P;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = NVIC_TIME_S;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void DrvTimerFcInit(void)
{
    TIM_CONF(1000);
    TIM_NVIC();

    /* TIM7 重新开时钟，开始计时 */
    RCC_APB1PeriphClockCmd(SYS_RCC_TIMx, ENABLE);
}
/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
				 
	

void initServoPWM(TIM_TypeDef* TIMx,uint32_t RCC_APB1Periph_TIMx)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


//开启TIMx时钟
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE);

//配置TIMx
TIM_DeInit(TIMx); 
TIM_TimeBaseStructure.TIM_Period = 2000;//20ms 50频率
TIM_TimeBaseStructure.TIM_Prescaler =819;//定时器分频系数，定时器时钟为82MHz，分频后得0.01MHz,即10kHz,0.1ms定时器加1
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
}

void initServoChannel(GPIO_TypeDef* GPIOx,uint32_t RCC_AHB1Periph_GPIOx,u16 GPIO_Pin_x,uint16_t GPIO_PinSourcex,uint8_t GPIO_AF_TIMx,TIM_TypeDef* TIMx,u8 ch)
{

RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
//配置复用功能，AGPIO1复用位TIMx
GPIO_PinAFConfig(GPIOx,GPIO_PinSourcex,GPIO_AF_TIMx);

//配置PWM输出管脚 初始化
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//设置为复用模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//复用推挽输出
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOx, &GPIO_InitStructure);

//PWM模式设置 与通道设置TIMx_CHx,每个定时器只有一个频率，每个通道可以设置不同的占空比
TIM_OCInitTypeDef TIM_OCInitStructure;
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//模式PWM1，无论定时器是向上计数还是向下计数，计数值>有效值时为有效电平。PWM2模式相反        
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//输出比较使能
//TIM_OCInitStructure.TIM_Pulse = servoInitAngle*200/180+50;//设置比较值，写CCRx，当计数器到达100时,电平发送跳变,即设置CCRx的值
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//设输出极性为高，即有效值为高电平,当计数器<200时,PWM为高电平

switch(ch)//选择pwm通道
{
case 1:
TIM_OC1Init(TIMx, &TIM_OCInitStructure);//通道2初始化
TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);//使能输出比较预装载
break;
case 2:
TIM_OC2Init(TIMx, &TIM_OCInitStructure);//通道初始化
TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);//使能输出比较预装载
break;
case 3:
TIM_OC3Init(TIMx, &TIM_OCInitStructure);//通道初始化
TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);//使能输出比较预装载
break;
case 4:
TIM_OC4Init(TIMx, &TIM_OCInitStructure);//通道初始化
TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);//使能输出比较预装载
break;
}
TIM_ARRPreloadConfig(TIMx, ENABLE);//使能自动重装载寄存器允许位
//开启TIMx
TIM_Cmd(TIMx, ENABLE); 
}


	

										  



