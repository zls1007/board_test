#include "led.h"


/****************************************************************************
*			GPIO���ú��� (��ͨ���)

*			PA15 --- led
*			PE10 --- ����������ʹ��
*
******************************************************************************/
void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	//��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	


	//led����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			//ѡ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//����Ϊ��ͨ���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//���Ƶ��Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//����Ϊ����   GPIO_PuPd_NOPULL(������)	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//����������ʹ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			//ѡ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;					//����Ϊ��ͨ���
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//���Ƶ��Ϊ100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;					//����Ϊ����   GPIO_PuPd_NOPULL(������)	
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
	//�ر�С��
	GPIO_SetBits(GPIOA,GPIO_Pin_15);
	
	//������������
	GPIO_SetBits(GPIOE,GPIO_Pin_10);


}





