#include "pid.h"

struct _pid{
	float SetSpeed;   		//定义设定值
	float ActualSpeed; 		//定义实际值
	float err; 						//定义偏差
	float err_next;				//定义上一次的偏差
	float err_last;				//定义再上次的偏差
	float Kp, Ki, Kd;			//定义比例、积分、微分系数
}pid;

void PID_init(void)
{
	pid.SetSpeed = 0.0;
	pid.ActualSpeed = 0.0;
	pid.err = 0.0;
	pid.err_last = 0.0;
	pid.err_next = 0.0;
	pid.Kp = 0.2;
	pid.Ki = 0.015;
	pid.Kd = 0.2;
}

float PID_realize(float speed)
{
	float Speed_inc;
	pid.SetSpeed = speed;
	pid.err = pid.SetSpeed - pid.ActualSpeed;
	//pid公式
	Speed_inc = pid.Kp*(pid.err-pid.err_next) + pid.Ki*pid.err + pid.Kd*(pid.err-2*pid.err_next+pid.err_last);
	pid.ActualSpeed += Speed_inc;
	
	//更新参数
	pid.err_last = pid.err_next;
	pid.err_next = pid.err;
	
	//返回实际值
	return pid.ActualSpeed;
}
