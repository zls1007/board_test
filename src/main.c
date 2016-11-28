/**
  ******************************************************************************
  * @file    main.c
  * @author  zhao linshan
  * @version V0.0.1
  * @date    2016-11-28
  * @brief   ������Ϊˮ�»�������Ӳ�����Գ���
	*
*/
#include "stm32f4xx.h"
#include "menu.h"
#include "timer.h"
#include "usart.h"
#include "led.h"

int main(void)
{

	
	UART7_Config();			//���Խӿ�
	TIM2_Init();     		//0.5s ��ʱ�ж�
	GPIO_Config();			//led ��ʾ

	Main_Menu();  			//��ʾ���˵�

}


