
#include "battery.h"
#include "usart.h"
#include "delay.h"

//==========================================
//读取字节
//==========================================
u16 Battery_Read_Word(u8 command)
{
	u8 i = 0;
	u8 read_buf[3];  	//暂存读取的数据
	u8 crc = 0x00;   	//crc校验值
	u16 word;					//读到的数据
	
	//开始读取
	I2C_Start();                				//起始信号
	I2C_Write(SlaveAddress);    	//发送设备地址+写信号（+0）
	I2C_Write(command);    				 	//发送存储单元地址
	I2C_Start();                				//起始信号
	I2C_Write(SlaveAddress+1); 	//发送设备地址+读信号（+1）	
	for(i = 0;i < 3;i++)
	{
		read_buf[i] = I2C_Read();
		if(i == 2)
			I2C_SendACK(1);			
		else
			I2C_SendACK(0);
	}
	I2C_Stop();			//读取完成
	
	delay_ms(2);
	//word = ((u16)read_buf[1]<<8) + read_buf[0];
	word = read_buf[1]*0xff + read_buf[0];
	//printf("value:%d, %d, %d\r\n", read_buf[0], read_buf[1], read_buf[2]);
	
	//crc 校验
	crc = GetCrc8(crc, SlaveAddress);
	crc = GetCrc8(crc, command);
	crc = GetCrc8(crc, SlaveAddress+1);
	crc = GetCrc8(crc, read_buf[0]);
	crc = GetCrc8(crc, read_buf[1]);
	if(crc != read_buf[2])
		return 0;
	else
	{
		//printf("crc ok\r\n");
		return word;
	}		
	
}

//==========================================
//读取半字
//==========================================
u8 Battery_Read_Halfword(u8 command)
{

	u8 i = 0;
	u8 read_buf[3];  	//暂存读取的数据
	u8 crc = 0x00;   	//crc校验值
	u16 word;					//读到的数据
	
	//开始读取
	I2C_Start();                				//起始信号
	I2C_Write(SlaveAddress);    	//发送设备地址+写信号（+0）
	I2C_Write(command);    				 	//发送存储单元地址
	I2C_Start();                				//起始信号
	I2C_Write(SlaveAddress+1); 	//发送设备地址+读信号（+1）	
	for(i = 0;i < 3;i++)
	{
		read_buf[i] = I2C_Read();
		if(i == 2)
			I2C_SendACK(1);			
		else
			I2C_SendACK(0);
	}
	I2C_Stop();			//读取完成
	
	delay_ms(1);

	//printf("value:%d\r\n", halfword);
	
	return read_buf[0];
}


//==========================================
//读取电池温度  单位：摄氏度
//若错误，返回-1
//==========================================
float GetBatteryTemper(void)
{
	u16 wold;
	float temper;
	wold = Battery_Read_Word(BatteryTemperCmd);
	
	if(wold == 0)  return -1;
	
	temper = (wold - 2731) * 0.1;
	return temper;
}

//==========================================
//读取电池电压 单位：v
//若错误，返回-1
//==========================================
float GetBatteryVoltage(void)
{
	u16 wold;
	float voltage;
	wold = Battery_Read_Word(BatteryVoltageCmd);
	
	if(wold == 0)  return -1;
	
	voltage = wold * 0.001;
	//printf("vol=%.2f\r\n", voltage);
	return voltage;
}

//==========================================
//读取电池电流  单位：a
//若错误，返回-1
//==========================================
float GetBatteryCurrent(void)
{
	u16 wold;
	float current;
	wold = Battery_Read_Word(BatteryCurrentCmd);
	
	if(wold == 0)  return -1;
	
	current = (s16)wold * 0.001;
	return current;
}

//==========================================
//读取电池平均电流  单位：a
//若错误，返回-1
//==========================================
float GetBatteryAverageCur(void)
{
	u16 wold;
	float current;
	wold = Battery_Read_Word(BatteryAverageCurCmd);
	
	if(wold == 0)  return -1;
	
	current = wold * 0.001;
	return current;
}

//==========================================
//读取电池相对容量百分比  单位：%
//若错误，返回-1
//==========================================
s8 GetBatteryRelativeCap(void)
{
	u8 halfwold;
	s8 capacity;
	halfwold = Battery_Read_Halfword(BatteryRelativeCapCmd);
	
	if(halfwold == 0xff)  return -1;
	
	capacity = halfwold;
	return capacity;
}

//==========================================
//读取电池绝对容量百分比  单位：%
//若错误，返回-1
//==========================================
s8 GetBatteryAbsoluteCap(void)
{
	u16 halfwold;
	s16 capacity;
	halfwold = Battery_Read_Halfword(BatteryAbsoluteCapCmd);
	
	if(halfwold == 0)  return -1;
	
	capacity = halfwold;
	return capacity;
}

//==========================================
//输出电池参数
//==========================================
void ReadBatteryInfo(void)
{
	float vol, cur, temper;
	s8 relcap, abscap;
	
	temper = GetBatteryTemper();
	vol = GetBatteryVoltage();
	cur = GetBatteryCurrent();

	relcap = GetBatteryRelativeCap();
	abscap = GetBatteryAbsoluteCap();
	printf("temper=%.2f, vol=%.2f, cur=%.2f, relcap=%d, abscap=%d\r\n", temper, vol, cur, relcap, abscap);
}
	

