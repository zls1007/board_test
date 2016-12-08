
#include "stm32f4xx_it.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include "seril_deal.h"

extern u8 usart_flag;
extern __IO u32 sys_tick;

/****************************************************************************
*										��ʱ��2�жϷ�������1us��ʱ�жϣ�
******************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{		
		//С����˸
		//GPIO_ToggleBits(GPIOA, GPIO_Pin_15);
		sys_tick ++;
		
		//��ձ�־λ
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
	}
}

/****************************************************************************
*										����1�жϷ�����(����λ��ͨ��)
******************************************************************************/ 
void USART1_IRQHandler(void)
{
	u8 c;
	static u8 buf[100];
	static u8 cnt = 0;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART1);
		
		buf[cnt ++] = c;
		if(c == '\n')
		{
			//buf[cnt] = '\0';
			if(usart_flag)
			{
				printf("USART1 receive: %s", buf);
			}
			memset(buf, 0, 16);
			cnt = 0;
		}
		else if(cnt > 90)
		{
			cnt = 0;
			memset(buf, 0, 100);
		}
	
		USART_SendData(USART1, c);
		//��ձ�־λ
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

/****************************************************************************
*										����2�жϷ�����(����λ��ͨ��)
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
				//printf("USART2 receive: %s \r\n", buf);
				usart_print(UART7, buf, cnt);
			}
			memset(buf, 0, 16);
			cnt = 0;
		}
	
		USART_SendData(USART2, c);
		//��ձ�־λ
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}

/****************************************************************************
*										����3�жϷ�����(���ۺϰ�ͨ��)
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
		if(cnt > 14 || c == '\n')
		{
			if(usart_flag)
			{
				//printf("USART3 receive: %s \r\n", buf);
				usart_print(UART7, buf, cnt);
			}
			memset(buf, 0, 16);
			cnt = 0;
		}

  	USART_SendData(USART3, c);   
   
		//��ձ�־λ
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}

/****************************************************************************
*										����4�жϷ�����
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
		if(cnt > 14 || c == '\n')
		{
			if(usart_flag)
			{
				//printf("UART4 receive: %s \r\n", buf);
				usart_print(UART7, buf, cnt);
			}
			memset(buf, 0, 16);
			cnt = 0;
		}
   
		//��ձ�־λ
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
}


/****************************************************************************
*										�δ�ʱ��������ʱ������
******************************************************************************/ 
extern u32 ntime;
void SysTick_Handler(void)
{
	ntime--;
}


