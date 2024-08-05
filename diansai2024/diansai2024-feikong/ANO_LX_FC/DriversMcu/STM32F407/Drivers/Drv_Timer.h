#ifndef _TIME_H_
#define _TIME_H_

#include "SysConfig.h"

void DrvTimerFcInit(void);
void initServoPWM(TIM_TypeDef* TIMx,uint32_t RCC_APB1Periph_TIMx);
void initServoChannel(GPIO_TypeDef* GPIOx,uint32_t RCC_AHB1Periph_GPIOx,u16 GPIO_Pin_x,uint16_t GPIO_PinSourcex,uint8_t GPIO_AF_TIMx,TIM_TypeDef* TIMx,u8 ch);
#endif
