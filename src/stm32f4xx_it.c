
#include "stm32f4xx_it.h"
#include "delay.h"
#include "usart.h"
#include <string.h>
#include "seril_deal.h"



/****************************************************************************
*										��ʱ��2�жϷ�������0.5s��ʱ�жϣ�
******************************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{		
		//С����˸
		GPIO_ToggleBits(GPIOA, GPIO_Pin_15);
		
		//��ձ�־λ
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);	
	}
}

/****************************************************************************
*										����2�жϷ�����(����λ��ͨ��)
******************************************************************************/ 
void USART2_IRQHandler(void)
{
	u8 c;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART2);
	
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
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(USART3);

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
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		c = USART_ReceiveData(UART4);
		 	
		USART_SendData(UART4, c);   
   
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


