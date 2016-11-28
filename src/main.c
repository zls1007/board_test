/**
  ******************************************************************************
  * @file    main.c
  * @author  zhao linshan
  * @version V0.0.1
  * @date    2016-11-28
  * @brief   本程序为水下机器人新硬件测试程序
	*
*/
#include "stm32f4xx.h"
#include "menu.h"
#include "timer.h"
#include "usart.h"
#include "led.h"

int main(void)
{

	
	UART7_Config();			//调试接口
	TIM2_Init();     		//0.5s 定时中断
	GPIO_Config();			//led 显示

	Main_Menu();  			//显示主菜单

}


