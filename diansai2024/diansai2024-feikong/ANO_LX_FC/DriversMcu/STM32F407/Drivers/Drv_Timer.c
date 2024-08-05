/******************** (C) COPYRIGHT 2017 ANO Tech ********************************
 * ����    �������ƴ�
 * ����    ��www.anotc.com
 * �Ա�    ��anotc.taobao.com
 * ����QȺ ��190169595
 * ����    ����ʱ������
**********************************************************************************/

#include "Drv_Timer.h"
#include "Drv_led.h"

#define SYS_TIMx_IRQn TIM7_IRQn
#define SYS_TIMx TIM7
#define SYS_RCC_TIMx RCC_APB1Periph_TIM7

void TIM_CONF(u16 period_ms) //APB1  84M
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

    /* ʹ��ʱ�� */
    RCC_APB1PeriphClockCmd(SYS_RCC_TIMx, ENABLE);

    TIM_DeInit(SYS_TIMx);

    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period = period_ms;

    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    /* ʱ��Ԥ��Ƶ��Ϊ */
    TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;

    /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //���ϼ���

    TIM_TimeBaseInit(SYS_TIMx, &TIM_TimeBaseStructure);

    TIM_ClearFlag(SYS_TIMx, TIM_FLAG_Update);

    TIM_ITConfig(SYS_TIMx, TIM_IT_Update, ENABLE);

    TIM_Cmd(SYS_TIMx, ENABLE);

    RCC_APB1PeriphClockCmd(SYS_RCC_TIMx, DISABLE); /*�ȹرյȴ�ʹ��*/
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

    /* TIM7 ���¿�ʱ�ӣ���ʼ��ʱ */
    RCC_APB1PeriphClockCmd(SYS_RCC_TIMx, ENABLE);
}
/******************* (C) COPYRIGHT 2014 ANO TECH *****END OF FILE************/
				 
	

void initServoPWM(TIM_TypeDef* TIMx,uint32_t RCC_APB1Periph_TIMx)
{
TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;


//����TIMxʱ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIMx, ENABLE);

//����TIMx
TIM_DeInit(TIMx); 
TIM_TimeBaseStructure.TIM_Period = 2000;//20ms 50Ƶ��
TIM_TimeBaseStructure.TIM_Prescaler =819;//��ʱ����Ƶϵ������ʱ��ʱ��Ϊ82MHz����Ƶ���0.01MHz,��10kHz,0.1ms��ʱ����1
TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
}

void initServoChannel(GPIO_TypeDef* GPIOx,uint32_t RCC_AHB1Periph_GPIOx,u16 GPIO_Pin_x,uint16_t GPIO_PinSourcex,uint8_t GPIO_AF_TIMx,TIM_TypeDef* TIMx,u8 ch)
{

RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOx, ENABLE);
//���ø��ù��ܣ�AGPIO1����λTIMx
GPIO_PinAFConfig(GPIOx,GPIO_PinSourcex,GPIO_AF_TIMx);

//����PWM����ܽ� ��ʼ��
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//����Ϊ����ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�����������
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOx, &GPIO_InitStructure);

//PWMģʽ���� ��ͨ������TIMx_CHx,ÿ����ʱ��ֻ��һ��Ƶ�ʣ�ÿ��ͨ���������ò�ͬ��ռ�ձ�
TIM_OCInitTypeDef TIM_OCInitStructure;
TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//ģʽPWM1�����۶�ʱ�������ϼ����������¼���������ֵ>��ЧֵʱΪ��Ч��ƽ��PWM2ģʽ�෴        
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//����Ƚ�ʹ��
//TIM_OCInitStructure.TIM_Pulse = servoInitAngle*200/180+50;//���ñȽ�ֵ��дCCRx��������������100ʱ,��ƽ��������,������CCRx��ֵ
TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//���������Ϊ�ߣ�����ЧֵΪ�ߵ�ƽ,��������<200ʱ,PWMΪ�ߵ�ƽ

switch(ch)//ѡ��pwmͨ��
{
case 1:
TIM_OC1Init(TIMx, &TIM_OCInitStructure);//ͨ��2��ʼ��
TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);//ʹ������Ƚ�Ԥװ��
break;
case 2:
TIM_OC2Init(TIMx, &TIM_OCInitStructure);//ͨ����ʼ��
TIM_OC2PreloadConfig(TIMx, TIM_OCPreload_Enable);//ʹ������Ƚ�Ԥװ��
break;
case 3:
TIM_OC3Init(TIMx, &TIM_OCInitStructure);//ͨ����ʼ��
TIM_OC3PreloadConfig(TIMx, TIM_OCPreload_Enable);//ʹ������Ƚ�Ԥװ��
break;
case 4:
TIM_OC4Init(TIMx, &TIM_OCInitStructure);//ͨ����ʼ��
TIM_OC4PreloadConfig(TIMx, TIM_OCPreload_Enable);//ʹ������Ƚ�Ԥװ��
break;
}
TIM_ARRPreloadConfig(TIMx, ENABLE);//ʹ���Զ���װ�ؼĴ�������λ
//����TIMx
TIM_Cmd(TIMx, ENABLE); 
}


	

										  



