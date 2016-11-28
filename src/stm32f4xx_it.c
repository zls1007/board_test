
#include "stm32f4xx_it.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include "seril_deal.h"



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
*										串口2中断服务函数(与上位机通信)
******************************************************************************/ 
void USART2_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART2);
	
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
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART3);

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
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(UART4);
		 	
		USART_SendData(UART4, c);   
   
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


