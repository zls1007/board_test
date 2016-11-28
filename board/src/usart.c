/**
  ******************************************************************************
  * @file    usart.c
  * @author  zhao linshan
  * @version V0.0.1
  * @date    2016-08-12
  * @brief   ���ļ���Ҫ�Ƕ�Ӳ��������Դ���г�ʼ������
	*			Ӳ����Դ��
  *          	USART2  115200-8-1-N  PD5-TXD  PD6-RXD  ����λ��ͨ�ţ�json��
	*						USART3  115200-8-1-N  PD8-TXD  PD9-RXD  ���Խӿ�		
	*						USART4  9600-8-1-N  	PA0-TXD  PA1-RXD  ���ۺϹ����ͨ��		
  *
	*			���ȼ�: (1��)
	*						USART2  0/4
	*						USART3  0/5
	*						USART4  0/6
							USRT7   0/7
	
	*			������
	*						USART2_Config(void);  				��USART2��������
	*						USART3_Config(void);  				��USART3��������
	*						USART4_Config(void);					��USART4��������
	*						int fputc(int ch, FILE *f);   USART3����ض���
	*						void SendStr(char *str);			��������ַ�������
*/


#include "usart.h"

/****************************************************************************
*			USART1 �������ú���
*			PB6 ---- TXD	
*			PB7 ---- RXD   
*			115200-8-1-N
******************************************************************************/
void USART1_Config(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
  
		//��ʱ��
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
  	
		//����IO����
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART1);  
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource7, GPIO_AF_USART1);
	
		//GPIO����
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	
		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
  	
		//USART1����
		USART_InitStructure.USART_BaudRate = 115200;				//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART1, &USART_InitStructure);
		USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);   		//��������ж�
  	USART_Cmd(USART1, ENABLE);
  	//USART_ClearFlag(USART3, USART_FLAG_TC);
		
		usart_print_str(USART1, "usart1 has inited, your input will be loop back!\r\n");
}

/****************************************************************************
*			USART2 �������ú���
*			PD5 ---- TXD	
*			PD6 ---- RXD   
*			115200-8-1-N
******************************************************************************/
void USART2_Config(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
  
		//��ʱ��
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
  	
		//����IO����
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);  
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);
	
		//GPIO����
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);    
	
		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
  	
		//USART2����
		USART_InitStructure.USART_BaudRate = 115200;				//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART2, &USART_InitStructure);
		USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);   		//��������ж�
  	USART_Cmd(USART2, ENABLE);
  	//USART_ClearFlag(USART3, USART_FLAG_TC);
		
		usart_print_str(USART2, "usart1 has inited, your input will be loop back!\r\n");
}


/****************************************************************************
*			USART3 �������ú���
*			PD8 ---- TXD	
*			PD9 ---- RXD   
*			115200-8-1-N
******************************************************************************/
void USART3_Config(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
  
		//��ʱ��
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  	
		//IO��ӳ��
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3);  
  	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);
	
		//GPIO����
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);  

		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
  	
		//����3����
		USART_InitStructure.USART_BaudRate = 115200;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(USART3, &USART_InitStructure);
		USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);   //��������ж�
  	USART_Cmd(USART3, ENABLE);
  	//USART_ClearFlag(USART3, USART_FLAG_TC);
		
		usart_print_str(USART3, "usart1 has inited, your input will be loop back!\r\n");
}

/****************************************************************************
*			UART4 �������ú���
*			PA0 ---- TXD	
*			PA1 ---- RXD   
*			9600-8-1-N
******************************************************************************/
void UART4_Config(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef  NVIC_InitStructure;
  
		//��ʱ��
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
  	
		//IO��ӳ��
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_UART4);  
  	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_UART4);
	
		//GPIO����
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);  

		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 6;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		
  	
		//����4����
		USART_InitStructure.USART_BaudRate = 115200;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(UART4, &USART_InitStructure);
		USART_ITConfig(UART4, USART_IT_RXNE,ENABLE);   //��������ж�
  	USART_Cmd(UART4, ENABLE);
  	//USART_ClearFlag(USART3, USART_FLAG_TC);
		
		usart_print_str(UART4, "usart1 has inited, your input will be loop back!\r\n");
}

/****************************************************************************
*			UART7 �������ú���
*			PE8 ---- TXD	
*			PE7 ---- RXD   
*			115200-8-1-N   �޽����ж�
******************************************************************************/
void UART7_Config(void)
{
  	GPIO_InitTypeDef GPIO_InitStructure;
  	USART_InitTypeDef USART_InitStructure;
  
		//��ʱ��
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART7, ENABLE);
  	
		//IO��ӳ��
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_UART7);  
  	GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_UART7);
	
		//GPIO����
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);  
  	
		//����7����
		USART_InitStructure.USART_BaudRate = 115200;//����������
  	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  	USART_InitStructure.USART_Parity = USART_Parity_No;
  	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; 
  	USART_Init(UART7, &USART_InitStructure);
		//USART_ITConfig(UART7, USART_IT_RXNE,ENABLE);   //��������ж�
  	USART_Cmd(UART7, ENABLE);
  	//USART_ClearFlag(USART3, USART_FLAG_TC);
}


/****************************************************************************
*			usart_deinit()  �ر����д���

******************************************************************************/
void usart_deinit(void)
{
		USART_ITConfig(USART1, USART_IT_RXNE,DISABLE);   
  	USART_Cmd(USART1, DISABLE);
	
		USART_ITConfig(USART2, USART_IT_RXNE,DISABLE);   
  	USART_Cmd(USART2, DISABLE);
	
		USART_ITConfig(USART3, USART_IT_RXNE,DISABLE);   
  	USART_Cmd(USART3, DISABLE);
	
		USART_ITConfig(UART4, USART_IT_RXNE,DISABLE);   
  	USART_Cmd(UART4, DISABLE);
}


/****************************************************************************
*			�������ӳ��
*			UART7 -- printf
******************************************************************************/
int fputc(int ch, FILE *f)
{
  	USART_SendData(UART7, (u8) ch);
  	while (USART_GetFlagStatus(UART7, USART_FLAG_TC) == RESET);
  	return ch;
}

/****************************************************************************
*		  ����ַ���
*			
******************************************************************************/
void usart_print_str(USART_TypeDef *usart, char *buf)
{
	while(*buf != '\0')
	{
		USART_SendData(usart, *buf);
  	while (USART_GetFlagStatus(usart, USART_FLAG_TC) == RESET);
		buf++;
	}
}



