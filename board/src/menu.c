#include "menu.h"
#include "delay.h"
#include "timer.h"
#include "usart.h"
#include "led.h"
#include "mpu9250.h"
#include "ms5611.h"
#include "adc.h"

//adc 转换的数据
__IO uint16_t ADC_ConvertedValue[2];

//姿态传感器接收到的数据
int16_t imu[10] = {0};

void led_test(void)
{
	int32_t num;
	
	//TIM3 pwm 初始化
	TIM3_Init();  			
	while(1)
	{
		SerialPutString("Please input the led light number[0-100]...(press a to abort)\n\r");
		if(GetIntegerInput(&num) == 0)
		{
			TIM_SetCompare1(TIM3, 0);
			TIM_SetCompare2(TIM3, 0);
			break;
		}
		else if(num < 0 || num > 100)
		{
			SerialPutString("Invalid Number ! ==> The number should interval 0 ~ 100\r");	
		}
		else
		{
			TIM_SetCompare1(TIM3, num*10);
			TIM_SetCompare2(TIM3, num*10);
		}
	}
}

void usart_test(void)
{
	uint8_t key = 0;	
	while(1)
	{
		SerialPutString("Please choose a device...\r\n\n");
		SerialPutString("  a:usart1   b:usart2  c:usart3  d:usart4  e:exit \r\n");
		key = GetKey();
		if(key == 'e')
		{
			usart_deinit();
			break;
		}
		switch(key)
		{
			case 'a':  usart_deinit();  USART1_Config(); break;
			case 'b':  usart_deinit();  USART2_Config(); break;
			case 'c':  usart_deinit();  USART3_Config(); break;
			case 'd':  usart_deinit();  UART4_Config(); break;
			default: SerialPutString("Invalid Input ! ==> The input should be either a,b,c,d or e\r"); break;
		}
	}
}

void adc_test(void)
{
	uint8_t cont = 50;
	float vol, deep;  //测得的电压、深度值
	
	//adc 初始化
	ADC3_Config();
	delay_ms(20);
	while(cont--)
	{
		vol = ADC_ConvertedValue[0] * 3.3 * 4/ 0xfff;
		deep = ADC_ConvertedValue[1] * 3.3 / 0xfff;
		deep = deep *1000/165;
		deep = (deep -4)*100/16;
		printf("voltage=%.2f, deep=%.2f\r\n", vol, deep);
		delay_ms(200);
	}
}

void moter_test(void)
{
	int32_t num;
	
	//TIM1 pwm 初始化
	TIM1_Init();  			
	while(1)
	{
		SerialPutString("Please input the moter speed [0-100]...(50 is the center, press 'a' to abort)\n\r");
		if(GetIntegerInput(&num) == 0)
		{
			TIM_SetCompare4(TIM1, 1522);
			TIM_SetCompare3(TIM1, 1522);
			TIM_SetCompare2(TIM1, 1522);
			TIM_SetCompare1(TIM1, 1522);
			break;
		}
		else if(num < 0 || num > 100)
		{
			SerialPutString("Invalid Number ! ==> The number should interval 0 ~ 100\r");	
		}
		else
		{
			num -= 50;
			num *= 9;
			num += 1522;
			TIM_SetCompare4(TIM1, num);
			TIM_SetCompare3(TIM1, num);
			TIM_SetCompare2(TIM1, num);
			TIM_SetCompare1(TIM1, num);
		}
	}
}

void mpu9250_test(void)
{
	uint8_t cont = 50;
	
	MPU9250_Config();   //mpu9250 初始化	
	delay_ms(100);
	while(cont--)
	{
		MPU9250_getData(imu);
		printf("value:%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",imu[0], imu[1], imu[2],imu[3], imu[4], imu[5], imu[6], imu[7], imu[8]);
		delay_ms(200);
	}
}

void ms5611_test(void)
{
	uint8_t cont = 50;
	
	float temper, pressure, altitude;
	
	 //大气压强模块初始化
   SPI2_Init();                                    //SPI1初始化
   SPI2_SetSpeed(SPI_BaudRatePrescaler_256);	   	 //设置SPI速率  128M/256=500KHZ
   MS5611_RESET();                                 //MS5611初始化
   MS5611_PROM_READ();                             //读取存储器(128-bit PROM)
	 delay_ms(100);
	while(cont--)
	{
		//采集气压计
		temper = MS5611_getTemperature(CMD_CONVERT_D2_OSR4096);
		pressure = MS5611_getPressure(CMD_CONVERT_D1_OSR4096);
		altitude = get_altitude();
		printf("pressure=%.3f  temper=%.3f\r\n", pressure, temper);
		delay_ms(200);
	}
}

void hmc5983_test(void)
{
	SerialPutString(" please waiting....\r\n");
}

void i2c_test(void)
{
	SerialPutString(" please waitint....\r\n");
}

void SDCard_test(void)
{
	SerialPutString(" please waitint....\r\n");
}


void Main_Menu(void)
{
	uint8_t key = 0;
	SerialPutString("\r\n======================================================================");
  SerialPutString("\r\n=              (C) COPYRIGHT 2016  PowerVision                       =");
  SerialPutString("\r\n=                                                                    =");
  SerialPutString("\r\n=  PowerRay  Integrated Management Board Application (Version 1.0.0) =");
  SerialPutString("\r\n=                                                                    =");
  SerialPutString("\r\n=                      By Zhao Linshan                     					=");
  SerialPutString("\r\n======================================================================");
  SerialPutString("\r\n\r\n");
	
	while(1)
	{
		SerialPutString("\r\n================== Main Menu ============================\r\n\n");
    SerialPutString("  led test-------------- 1\r\n\n");
		SerialPutString("  usart test------------ 2\r\n\n");
    SerialPutString("  adc test-------------- 3\r\n\n");
		SerialPutString("  moter test------------ 4\r\n\n");
		SerialPutString("  mpu9250 test---------- 5\r\n\n");
    SerialPutString("  ms5611 test----------- 6\r\n\n");
		SerialPutString("  hmc5983 test---------- 7\r\n\n");
		SerialPutString("  i2c test-------------- 8\r\n\n");
		SerialPutString("  sd card test---------- 9\r\n\n");
		SerialPutString("==========================================================\r\n\n");
		
		//获取用户输入
		key = GetKey();
		switch(key)
		{
			case '1': led_test();  		break;
			case '2': usart_test(); 	break;
			case '3': adc_test();			break;
			case '4': moter_test(); 	break;
			case '5': mpu9250_test(); break;
			case '6': ms5611_test();	break;
			case '7': hmc5983_test();	break;
			case '8': i2c_test();			break;
			case '9': SDCard_test();	break;
			default: SerialPutString("Invalid Number ! ==> The number should be either 1 ~ 9\r"); break;
		}	
    
	}
	
}
