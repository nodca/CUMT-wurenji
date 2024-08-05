
#include "bsp_pid.h"

//����ȫ�ֱ���

_pid pid_speed, pid_speed2,pid_speed3,pid_speed4;    
_pid pid_angel;   
_pid pid_location, pid_location2;



/**
  * @brief  PID������ʼ��
	*	@note 	��
  * @retval ��
  */
void PID_param_init()
{

  
  	/* �ٶ���س�ʼ������ */
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
		
		/*�Ƕ�PID��ʼ��*/
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
    set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, pid_temp, 3);     // ��ͨ�� 1 ���� P I D ֵ
#endif
}

/**
  * @brief  ����Ŀ��ֵ
  * @param  val		Ŀ��ֵ
	*	@note 	��
  * @retval ��
  */
void set_pid_target(_pid *pid, float temp_val)
{
  pid->target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}

/**
  * @brief  ��ȡĿ��ֵ
  * @param  ��
	*	@note 	��
  * @retval Ŀ��ֵ
  */
float get_pid_target(_pid *pid)
{
  return pid->target_val;    // ���õ�ǰ��Ŀ��ֵ
}

void set_p_i_d(_pid *pid, float p, float i, float d)
{
  	pid->Kp = p;    // ���ñ���ϵ�� P
		pid->Ki = i;    // ���û���ϵ�� I
		pid->Kd = d;    // ����΢��ϵ�� D
}

/**
  * @brief  λ��PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
  */
float location_pid_realize(_pid *pid, float actual_val)  
{
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err=pid->target_val-actual_val;
  
//    /* �趨�ջ����� */    
//    if((pid->err >= -0.1) && (pid->err <= 0.1)) 
//    {
//      pid->err = 0;
//      pid->integral = 0;
//    }
    
    pid->integral += pid->err;    // ����ۻ�

		/*PID�㷨ʵ��*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                  +pid->Kd*(pid->err-pid->err_last);
  
		/*����*/
    pid->err_last=pid->err;
    
		/*���ص�ǰʵ��ֵ*/
    return pid->actual_val;
}

/**
  * @brief  �ٶ�PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
	*	@note 	��
  * @retval ͨ��PID���������
  */
float speed_pid_realize(_pid *pid, float actual_val)
{
		/*����Ŀ��ֵ��ʵ��ֵ�����*/
    pid->err=pid->target_val-actual_val;

	
		
    pid->integral += pid->err;    // ����ۻ�
	
	
	  /*�����޷�*/
	   	 if (pid->integral >= 1000) {pid->integral =1000;}
      else if (pid->integral < -1000)  {pid->integral = -1000;}

		/*PID�㷨ʵ��*/
    pid->actual_val = pid->Kp*pid->err
		                  +pid->Ki*pid->integral
		                   +pid->Kd*(pid->err-pid->err_last);
  
		/*����*/
    pid->err_last=pid->err;
    
		/*���ص�ǰʵ��ֵ*/
    return pid->actual_val;
}

float angel_pid(_pid *pid,float actual_val){
	pid->err=pid->target_val-actual_val;
	pid->actual_val=pid->Kp*pid->err+pid->Kd*(pid->err-pid->err_last);
	pid->err_last=pid->err;
	return pid->actual_val;
}


