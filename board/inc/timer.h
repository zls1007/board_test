#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f4xx_conf.h"

void TIM1_Init(void);  //pwm 4路输出   电机控制
void TIM2_Init(void);  //pwm 2路输出	 灯光控制
void TIM3_Init(void);  //定时
void TIM4_Init(void);  //ppm输入捕获

#endif
