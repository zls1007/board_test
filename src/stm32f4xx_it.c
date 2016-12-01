
#include "stm32f4xx_it.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include "seril_deal.h"

extern u8 usart_flag;

/****************************************************************************
*										定时器2中断服务函数（0.5s定时中断）
******************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{		
		//小灯闪烁
		GPIO_ToggleBits(GPIOA, GPIO_Pin_15);
		
		//清空标志位
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
	}
}

/****************************************************************************
*										串口1中断服务函数(与上位机通信)
******************************************************************************/ 
void USART1_IRQHandler(void)
{
	u8 c;
	static u8 buf[16];
	static u8 cnt = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART1);
		
		buf[cnt ++] = c;
		if(cnt > 15 || c == '\n')
		{
			if(usart_flag)
			{
				printf("USART1 receive: %s \r\n", buf);
			}
			memset(buf, 0, 16);
			cnt = 0;
		}
	
		USART_SendData(USART1, c);
		//清空标志位
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

/****************************************************************************
*										串口2中断服务函数(与上位机通信)
******************************************************************************/ 
void USART2_IRQHandler(void)
{
	u8 c;
	static u8 buf[16];
	static u8 cnt = 0;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART2);
		
		buf[cnt ++] = c;
		if(cnt > 15 || c == '\n')
		{
			if(usart_flag)
			{
				printf("USART2 receive: %s \r\n", buf);
			}
			memset(buf, 0, 16);
			cnt = 0;
		}
	
		USART_SendData(USART2, c);
		//清空标志位
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

/****************************************************************************
*										串口3中断服务函数(与综合板通信)
******************************************************************************/ 
void USART3_IRQHandler(void)
{
	u8 c;
	static u8 buf[16];
	static u8 cnt = 0;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART3);
		
		buf[cnt ++] = c;
		if(cnt > 15 || c == '\n')
		{
			if(usart_flag)
			{
				printf("USART3 receive: %s \r\n", buf);
			}
			memset(buf, 0, 16);
			cnt = 0;
		}

  	USART_SendData(USART3, c);   
   
		//清空标志位
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

/****************************************************************************
*										串口4中断服务函数
******************************************************************************/ 
void UART4_IRQHandler(void)
{
	u8 c;
	static u8 buf[16];
	static u8 cnt = 0;
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(UART4);
		 	
		USART_SendData(UART4, c);   
		
		buf[cnt ++] = c;
		if(cnt > 15 || c == '\n')
		{
			if(usart_flag)
			{
				printf("UART4 receive: %s \r\n", buf);
			}
			memset(buf, 0, 16);
			cnt = 0;
		}
   
		//清空标志位
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
}


/****************************************************************************
*										滴答定时器（产生时间间隔）
******************************************************************************/ 
extern u32 ntime;
void SysTick_Handler(void)
{
	ntime--;
}


