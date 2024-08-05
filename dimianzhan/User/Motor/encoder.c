
#include "encoder.h"
#include "stm32f1xx.h"
#include "tim.h"
#include "usart.h"

long g_lMotor1PulseSigma =0;
long g_lMotor2PulseSigma=0;
long g_lMotor3PulseSigma =0;
long g_lMotor4PulseSigma=0;
short g_nMotor1Pulse=0,g_nMotor2Pulse=0,g_nMotor3Pulse=0,g_nMotor4Pulse=0;//ȫ�ֱ����� ������������ֵ




/*******************ʵ������ʱ��ȡ��������ֵ************************/   
void GetMotorPulse(void)//��ȡ�������
{
	g_nMotor1Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim8));//��ȡ������ֵ   
g_nMotor1Pulse=-g_nMotor1Pulse;
	__HAL_TIM_SET_COUNTER(&htim8,0);//TIM8����������
	
	g_nMotor2Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim5));//��ȡ������ֵ
//	g_nMotor2Pulse = -g_nMotor2Pulse;
	__HAL_TIM_SET_COUNTER(&htim5,0);//TIM5����������
	
	g_nMotor3Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim4));//��ȡ������ֵ   
	//g_nMotor3Pulse=-g_nMotor3Pulse;
	__HAL_TIM_SET_COUNTER(&htim4,0);//TIM4���������� 
	
	g_nMotor4Pulse = (short)(__HAL_TIM_GET_COUNTER(&htim3));//��ȡ������ֵ
	//g_nMotor4Pulse = -g_nMotor4Pulse;
	__HAL_TIM_SET_COUNTER(&htim3,0);//TIM3����������
	
	
//	g_lMotor1PulseSigma += g_nMotor1Pulse;//λ���⻷ʹ�õ������ۻ�     
	//g_lMotor2PulseSigma += g_nMotor2Pulse;//λ���⻷ʹ�õ������ۻ�   

}
