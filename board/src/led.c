#include "led.h"


/****************************************************************************
*			GPIO配置函数 (普通输出)

*			PA15 --- led
*			PE10 --- 传感器供电使能
*
******************************************************************************/
void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	//打开时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	


	//led配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			//选定引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//设置为普通输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//输出频率为100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//设置为上拉   GPIO_PuPd_NOPULL(不上拉)	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//传感器供电使能配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//选定引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//设置为普通输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//输出频率为100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//设置为上拉   GPIO_PuPd_NOPULL(不上拉)	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	//关闭小灯
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	
	//给传感器供电
	GPIO_SetBits(GPIOE,GPIO_Pin_10);


}





