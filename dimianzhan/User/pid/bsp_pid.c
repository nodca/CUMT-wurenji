
#include "bsp_pid.h"

//定义全局变量

_pid pid_speed, pid_speed2,pid_speed3,pid_speed4;    
_pid pid_angel;   
_pid pid_location, pid_location2;



/**
  * @brief  PID参数初始化
	*	@note 	无
  * @retval 无
  */
void PID_param_init()
{

  
  	/* 速度相关初始化参数 */
    pid_speed.target_val=0.0;				
    pid_speed.actual_val=0.0;
    pid_speed.err=0.0;
    pid_speed.err_last=0.0;
    pid_speed.integral=0.0;
  
		pid_speed.Kp = 15.8;  //
		pid_speed.Ki = 5.8;  //
		pid_speed.Kd = 0.0;  //
		
    pid_speed2.target_val=0.0;				
    pid_speed2.actual_val=0.0;
    pid_speed2.err=0.0;
    pid_speed2.err_last=0.0;
    pid_speed2.integral=0.0;
  
		pid_speed2.Kp = 15.8;  
		pid_speed2.Ki = 5.8;  
		pid_speed2.Kd = 0.0;  
		
		pid_speed3.target_val=0.0;				
    pid_speed3.actual_val=0.0;
    pid_speed3.err=0.0;
    pid_speed3.err_last=0.0;
    pid_speed3.integral=0.0;
  
		pid_speed3.Kp = 15.8;  
		pid_speed3.Ki = 5.8;  
		pid_speed3.Kd = 0.0;  
		
    pid_speed4.target_val=0.0;				
    pid_speed4.actual_val=0.0;
    pid_speed4.err=0.0;
    pid_speed4.err_last=0.0;
    pid_speed4.integral=0.0;
  
		pid_speed4.Kp = 15.8;  
		pid_speed4.Ki = 5.8;  
		pid_speed4.Kd = 0.0; 
		
		/*角度PID初始化*/
		pid_angel.target_val=0.0;				
    pid_angel.actual_val=0.0;
    pid_angel.err=0.0;
    pid_angel.err_last=0.0;
    pid_angel.integral=0.0;
  
		pid_angel.Kp = 1.0;  //
		pid_angel.Ki = 0.0;  //
		pid_angel.Kd = 0.5;  //
		

		
#if defined(PID_ASSISTANT_EN)
    float pid_temp[3] = {pid.Kp, pid.Ki, pid.Kd};   
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // 给通道 1 发送 P I D 值
#endif
}

/**
  * @brief  设置目标值
  * @param  val		目标值
	*	@note 	无
  * @retval 无
  */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // 设置当前的目标值
}

/**
  * @brief  获取目标值
  * @param  无
	*	@note 	无
  * @retval 目标值
  */
float get_pid_target(_pid *pid)
{
  return pid->target_val;    // 设置当前的目标值
}

void set_p_i_d(_pid *pid, float p, float i, float d)
{
  	pid->Kp = p;    // 设置比例系数 P
		pid->Ki = i;    // 设置积分系数 I
		pid->Kd = d;    // 设置微分系数 D
}

/**
  * @brief  位置PID算法实现
  * @param  actual_val:实际值
	*	@note 	无
  * @retval 通过PID计算后的输出
  */
float location_pid_realize(_pid *pid, float actual_val)  
{
		/*计算目标值与实际值的误差*/
    pid->err=pid->target_val-actual_val;
  
//    /* 设定闭环死区 */    
//    if((pid->err >= -0.1) && (pid->err <= 0.1)) 
//    {
//      pid->err = 0;
//      pid->integral = 0;
//    }
    
    pid->integral += pid->err;    // 误差累积

		/*PID算法实现*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                  +pid->Kd*(pid->err-pid->err_last);
  
		/*误差传递*/
    pid->err_last=pid->err;
    
		/*返回当前实际值*/
    return pid->actual_val;
}

/**
  * @brief  速度PID算法实现
  * @param  actual_val:实际值
	*	@note 	无
  * @retval 通过PID计算后的输出
  */
float speed_pid_realize(_pid *pid, float actual_val)
{
		/*计算目标值与实际值的误差*/
    pid->err=pid->target_val-actual_val;

	
		
    pid->integral += pid->err;    // 误差累积
	
	
	  /*积分限幅*/
	   	 if (pid->integral >= 1000) {pid->integral =1000;}
      else if (pid->integral < -1000)  {pid->integral = -1000;}

		/*PID算法实现*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                   +pid->Kd*(pid->err-pid->err_last);
  
		/*误差传递*/
    pid->err_last=pid->err;
    
		/*返回当前实际值*/
    return pid->actual_val;
}

float angel_pid(_pid *pid,float actual_val){
	pid->err=pid->target_val-actual_val;
	pid->actual_val=pid->Kp*pid->err+pid->Kd*(pid->err-pid->err_last);
	pid->err_last=pid->err;
	return pid->actual_val;
}


