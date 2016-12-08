/**
  ******************************************************************************
  * @file    timer.c
  * @author  zhao linshan
  * @version V0.0.1
  * @date    2016-08-12
  * @brief   本文件主要是对硬件定时器资源进行初始化配置
	*			硬件资源：	
	*						TIM1  高级定时器   PWM输出模式   对无刷电调进行控制
	*						TIM3  通用定时器   时基单元模式  定时中断计时
	*						TIM4  通用定时器 	 输入捕获模式	 对ppm信号进行捕获						
  *
	*			优先级: (1组)
	*						TIM1  0/3   （非中断模式）
	*						TIM3  0/2
	*						TIM4  0/1
	
	*			函数：
	*						void TIM1_Init(void)     	TIM1定时器pwm输出设置（电调）
	*						void TIM3_Init(void)			TIM3定时器基本时基单元（定时0.5s）
	*						void TIM4_Init(void)			TIM4定时器输入捕获设置（ppm）
*/

#include "timer.h"

/****************************************************************************
*			TIM1 PWM输出模式（对无刷电调进行控制）
*
*			对168MHz（TIM1时钟源为168MHz）
*					分频数 ———— 168  
*					计数值 ———— 20000  20ms
*     占空比变化 ———— 1000 ~ 2000  1.0ms ~ 2.0ms
*     占空比初值 1500
*			输出通道：
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
		
		//打开时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
		//GPIO设置  PWM 复用输出
//		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 |GPIO_Pin_13 |GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOE, &GPIO_InitStructure);
	
		//GPIO  IO  推挽输出PE9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			//选定引脚
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//设置为普通输出
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//输出频率为100MHz
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//设置为上拉   GPIO_PuPd_NOPULL(不上拉)	
		GPIO_Init(GPIOE,&GPIO_InitStructure);
		//GPIO  IO  推挽输出PD13 PD14
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;			//选定引脚
		GPIO_Init(GPIOD,&GPIO_InitStructure);
		
		//低电平输出
		GPIO_ResetBits(GPIOE,GPIO_Pin_9);
		GPIO_ResetBits(GPIOD,GPIO_Pin_13);
		GPIO_ResetBits(GPIOD,GPIO_Pin_14);

		//重定向IO口
//		GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
		GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);

		//中断优先级设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		//基本时基单元设置
		TIM_DeInit(TIM1);  
		TIM_TimeBaseStructure.TIM_Period = 20000;
		TIM_TimeBaseStructure.TIM_Prescaler = 168 - 1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

		TIM_OCStructInit(&TIM_OCInitStructure);  
		//通道1
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//		TIM_OC1Init(TIM1, &TIM_OCInitStructure);
//		TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//通道2 	
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC2Init(TIM1, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//通道3
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC3Init(TIM1, &TIM_OCInitStructure);
		TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
		//通道4
		TIM_OCInitStructure.TIM_Pulse = 1522;
		TIM_OC4Init(TIM1, &TIM_OCInitStructure);
		TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
		
		//使能时钟
		TIM_Cmd(TIM1, ENABLE);		
		TIM_CtrlPWMOutputs(TIM1, ENABLE);  //使能TIM1的PWM输出
}

/****************************************************************************
*			TIM3 PWM输出模式(对灯光亮度进行控制)
*
*			对84MHz（TIM3时钟源为84MHz）
*					分频数 ———— 84  
*					计数值 ———— 1000  1KHz
*     占空比变化 ———— 0 ~ 1000  
*     占空比初值 200
*			输出通道：
*					PC6  CH1
*					PC7	 CH2

******************************************************************************/
void TIM3_Init(void)
{	 
		GPIO_InitTypeDef GPIO_InitStructure;	
		NVIC_InitTypeDef  NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_OCInitTypeDef  TIM_OCInitStructure;
		
		//打开时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	
		//GPIO设置
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOC, &GPIO_InitStructure);

		//重定向IO口
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
		GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);


		//中断优先级设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		//基本时基单元设置
		TIM_DeInit(TIM3);  
		TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
		TIM_TimeBaseStructure.TIM_Prescaler = 84 - 1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

		TIM_OCStructInit(&TIM_OCInitStructure);  
		//通道1
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse = 200;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		TIM_OC1Init(TIM3, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
		//通道2 	
		TIM_OCInitStructure.TIM_Pulse = 200;
		TIM_OC2Init(TIM3, &TIM_OCInitStructure);
		TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

		
		//使能时钟
		TIM_Cmd(TIM3, ENABLE);		
		TIM_CtrlPWMOutputs(TIM3, ENABLE);  //使能TIM3的PWM输出
}

/****************************************************************************
*			TIM2 时钟中断
*
*			对84MHz（TIM3时钟源为168MHz/2）   8M
*			0.5s  定时中断
******************************************************************************/
void TIM2_Init(void)
{
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		NVIC_InitTypeDef  NVIC_InitStructure; 

		//打开时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  

		//中断优先级设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);		

		//定时器设置
		TIM_TimeBaseStructure.TIM_Period = 1000;
		TIM_TimeBaseStructure.TIM_Prescaler = 42000-1;
		TIM_TimeBaseStructure.TIM_ClockDivision = 0;
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM2, ENABLE);
}


/****************************************************************************
*			TIM4 输入捕获设置  ppm捕获
*
*			对84MHz（TIM3时钟源为168MHz/2）
*					分频数：84  1us 计数值加1
*					溢出值：65536  上升沿
*			输入端口
*     		PD14  TIM4_CH3  ppm捕获
* 		ppm  8通道 1~2ms  对应1000~2000
******************************************************************************/
void TIM4_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		TIM_ICInitTypeDef  TIM_ICInitStructure;
		
		//打开时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

		//GPIO设置
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14 | GPIO_Pin_13;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_Init(GPIOD, &GPIO_InitStructure);
		GPIO_SetBits(GPIOD,GPIO_Pin_14);
		
		//重定向IO口
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
		GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);

		//中断优先级设置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		//初始化时基单元
		TIM_TimeBaseStructure.TIM_Period = 65535; 
		TIM_TimeBaseStructure.TIM_Prescaler =84-1; 
		TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
		TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
	
		//输入捕获设置ppm
		TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
		TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;   
		TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
		TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
		TIM_ICInitStructure.TIM_ICFilter = 0x0;
		TIM_ICInit(TIM4, &TIM_ICInitStructure);
		
		//使能中断
		TIM_ITConfig(TIM4,TIM_IT_Update|TIM_IT_CC3,ENABLE);

		//使能定时器
		TIM_Cmd(TIM4,ENABLE); 
}



