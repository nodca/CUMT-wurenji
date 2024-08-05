#include "control.h"
#include "bsp_pid.h"
#include "stm32f1xx_it.h"
#include "main.h"
#include "tim.h"
#include "Fire_protocol.h"
int outval1,outval2,outval3,outval4;
void speed_set(float speed_x,float speed_y,float speed_z){
	set_pid_target(&pid_speed, speed_y + speed_x - speed_z);
	set_pid_target(&pid_speed2, speed_y - speed_x + speed_z);
	set_pid_target(&pid_speed3, speed_y - speed_x - speed_z);
	set_pid_target(&pid_speed4, speed_y + speed_x + speed_z);
	
}

void motor_pwm(void){
	//set_pid_target(&pid_speed, 20);
	//set_pid_target(&pid_speed, 20);
	//set_pid_target(&pid_speed2, 20);
	//set_pid_target(&pid_speed3, 20);
	//set_pid_target(&pid_speed4, 20);
 outval1=speed_pid_realize(&pid_speed, actual_speed1);
	outval2=speed_pid_realize(&pid_speed2, actual_speed2);
	outval3=speed_pid_realize(&pid_speed3, actual_speed3);
	outval4=speed_pid_realize(&pid_speed4, actual_speed4);
	//限幅
	if(outval1>=1000) outval1=outval1-100;
	else if(outval1<=-1000) outval1=outval1+100;
		if(outval2>=1000) outval2=outval2-100;
	else if(outval2<=-1000) outval2=outval2+100;
		if(outval3>=1000) outval3=outval3-100;
	else if(outval3<=-1000) outval3=outval3+100;
		if(outval4>=1000) outval1=outval4-100;
	else if(outval4<=-1000) outval1=outval4+100;
	//outval1=250;
	//电机一
	if(outval1>=0){
	 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,outval1) ;
	 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,0) ;
	}
	else if(outval1<0){
		outval1=-outval1;
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0) ;
	 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,outval1) ;
	}
	//电机二
		if(outval2>=0){
	 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,outval2) ;
	 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,0) ;
	}
	else if(outval2<0){
		outval2=-outval2;
		__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,0) ;
	 __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_4,outval2) ;
	}
	//电机三
		if(outval3>=0){
			
	 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,outval3) ;
	 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,0) ;
	}
	else if(outval3<0){
		outval3=-outval3;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,0) ;
	 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,outval3) ;
	}
	//电机四
		if(outval4>=0){
	 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,outval4) ;
	 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,0) ;
	}
	else if(outval4<0){
		outval4=-outval4;
		__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_3,0) ;
	 __HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_4,outval4) ;
	}
	
	  #if defined(PID_ASSISTANT_EN)
    set_computer_value(SEND_FACT_CMD, CURVES_CH1, &actual_speed1, 1);                // 给通道 1 发送实际值
//		set_computer_value(SEND_TARGET_CMD, CURVES_CH1,&TargetSpeed, 1);                // 给通道 1 发送目标值
  #endif
}
