/**
  ******************************************************************************
  * @file    timer.c
  * @author  zhao linshan
  * @version V0.0.1
  * @date    2016-08-12
  * @brief   ���ļ���Ҫ�Ƕ�Ӳ����ʱ����Դ���г�ʼ������
	*			Ӳ����Դ��	
	*						TIM1  �߼���ʱ��   PWM���ģʽ   ����ˢ������п���
	*						TIM3  ͨ�ö�ʱ��   ʱ����Ԫģʽ  ��ʱ�жϼ�ʱ
	*						TIM4  ͨ�ö�ʱ�� 	 ���벶��ģʽ	 ��ppm�źŽ��в���						
  *
	*			���ȼ�: (1��)
	*						TIM1  0/3   �����ж�ģʽ��
	*						TIM3  0/2
	*						TIM4  0/1
	
	*			������
	*						void TIM1_Init(void)     	TIM1��ʱ��pwm������ã������
	*						void TIM3_Init(void)			TIM3��ʱ������ʱ����Ԫ����ʱ0.5s��
	*						void TIM4_Init(void)			TIM4��ʱ�����벶�����ã�ppm��
*/

#include "timer.h"

/****************************************************************************
*			TIM1 PWM���ģʽ������ˢ������п��ƣ�
*
*			��168MHz��TIM1ʱ��ԴΪ168MHz��
*					��Ƶ�� �������� 168  
*					����ֵ �������� 20000  20ms
*     ռ�ձȱ仯 �������� 1000 ~ 2000  1.0ms ~ 2.0ms
*     ռ�ձȳ�ֵ 1500
*			���ͨ����
*					PE14  CH1
*					PE13	CH2
*					PE11	CH3
*			//		PE9   CH4  GND
*			//    PD13	GND
*			//		PD14  GND
******************************************************************************/
void TIM1_Init(void)
{	 
		GPIO_InitTypeDef GPIO_InitStructure;	
		NVIC_InitTypeDef  NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		//��ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
		//GPIO����  PWM �������
//		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		//GPIO  IO  �������PE9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//ѡ������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//����Ϊ��ͨ���
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//���Ƶ��Ϊ100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//����Ϊ����   GPIO_PuPd_NOPULL(������)	
		GPIO_Init(GPIOE,&GPIO_InitStructure);
		//GPIO  IO  �������PD13 PD14
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;			//ѡ������
		GPIO_Init(GPIOD,&GPIO_InitStructure);
		
		//�͵�ƽ���
		GPIO_ResetBits(GPIOE,GPIO_Pin_9);
		GPIO_ResetBits(GPIOD,GPIO_Pin_13);
		GPIO_ResetBits(GPIOD,GPIO_Pin_14);

		//�ض���IO��
//		GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		//����ʱ����Ԫ����
		TIM_DeInit(TIM1);  
		TIM_TimeBaseStructure.TIM_Period = 20000;
		TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

		TIM_OCStructInit(&TIM_OCInitStructure);  
		//ͨ��1
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
//		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//ͨ��2 	
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//ͨ��3
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//ͨ��4
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
		
		//ʹ��ʱ��
		TIM_Cmd(TIM1, ENABLE);		
		TIM_CtrlPWMOutputs(TIM1, ENABLE);  //ʹ��TIM1��PWM���
}

/****************************************************************************
*			TIM3 PWM���ģʽ(�Եƹ����Ƚ��п���)
*
*			��84MHz��TIM3ʱ��ԴΪ84MHz��
*					��Ƶ�� �������� 84  
*					����ֵ �������� 1000  1KHz
*     ռ�ձȱ仯 �������� 0 ~ 1000  
*     ռ�ձȳ�ֵ 200
*			���ͨ����
*					PC6  CH1
*					PC7	 CH2

******************************************************************************/
void TIM3_Init(void)
{	 
		GPIO_InitTypeDef GPIO_InitStructure;	
		NVIC_InitTypeDef  NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		//��ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
		//GPIO����
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		//�ض���IO��
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);


		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		//����ʱ����Ԫ����
		TIM_DeInit(TIM3);  
		TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
		TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		TIM_OCStructInit(&TIM_OCInitStructure);  
		//ͨ��1
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		//ͨ��2 	
		TIM_OCInitStructure.TIM_Pulse = 0;
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

		
		//ʹ��ʱ��
		TIM_Cmd(TIM3, ENABLE);		
		TIM_CtrlPWMOutputs(TIM3, ENABLE);  //ʹ��TIM3��PWM���
}

/****************************************************************************
*			TIM2 ʱ���ж�
*
*			��84MHz��TIM3ʱ��ԴΪ168MHz/2��   8M
*			0.5s  ��ʱ�ж�
******************************************************************************/
void TIM2_Init(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef  NVIC_InitStructure; 

		//��ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  

		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		//��ʱ������
		TIM_TimeBaseStructure.TIM_Period = 1000;
		TIM_TimeBaseStructure.TIM_Prescaler = 42000-1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM2, ENABLE);
}


/****************************************************************************
*			TIM4 ���벶������  ppm����
*
*			��84MHz��TIM3ʱ��ԴΪ168MHz/2��
*					��Ƶ����84  1us ����ֵ��1
*					���ֵ��65536  ������
*			����˿�
*     		PD14  TIM4_CH3  ppm����
* 		ppm  8ͨ�� 1~2ms  ��Ӧ1000~2000
******************************************************************************/
void TIM4_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_ICInitTypeDef  TIM_ICInitStructure;
		
		//��ʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		//GPIO����
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		GPIO_SetBits(GPIOD,GPIO_Pin_14);
		
		//�ض���IO��
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);

		//�ж����ȼ�����
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		//��ʼ��ʱ����Ԫ
		TIM_TimeBaseStructure.TIM_Period = 65535; 
		TIM_TimeBaseStructure.TIM_Prescaler =84-1; 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	
		//���벶������ppm
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;   
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0x0;
		TIM_ICInit(TIM4, &TIM_ICInitStructure);
		
		//ʹ���ж�
		TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC3,ENABLE);

		//ʹ�ܶ�ʱ��
		TIM_Cmd(TIM4,ENABLE); 
}



