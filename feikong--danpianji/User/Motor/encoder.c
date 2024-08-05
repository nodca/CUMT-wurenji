
#include "encoder.h"
#include "stm32f1xx.h"
#include "tim.h"
#include "usart.h"

long g_lMotor1PulseSigma =0;
long g_lMotor2PulseSigma=0;
long g_lMotor3PulseSigma =0;
long g_lMotor4PulseSigma=0;
short g_nMotor1Pulse=0,g_nMotor2Pulse=0,g_nMotor3Pulse=0,g_nMotor4Pulse=0;//全局变量， 保存电机脉冲数值




/*******************实际运行时读取编码器数值************************/   
void GetMotorPulse(void)//读取电机脉冲
{
	g_nMotor1Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim8));//获取计数器值   
g_nMotor1Pulse=-g_nMotor1Pulse;
	__HAL_TIM_SET_COUNTER(&htim8,0);//TIM8计数器清零
	
	g_nMotor2Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim5));//获取计数器值
//	g_nMotor2Pulse = -g_nMotor2Pulse;
	__HAL_TIM_SET_COUNTER(&htim5,0);//TIM5计数器清零
	
	g_nMotor3Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim4));//获取计数器值   
	//g_nMotor3Pulse=-g_nMotor3Pulse;
	__HAL_TIM_SET_COUNTER(&htim4,0);//TIM4计数器清零 
	
	g_nMotor4Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim3));//获取计数器值
	//g_nMotor4Pulse = -g_nMotor4Pulse;
	__HAL_TIM_SET_COUNTER(&htim3,0);//TIM3计数器清零
	
	
//	g_lMotor1PulseSigma += g_nMotor1Pulse;//位置外环使用的脉冲累积     
	//g_lMotor2PulseSigma += g_nMotor2Pulse;//位置外环使用的脉冲累积   

}
