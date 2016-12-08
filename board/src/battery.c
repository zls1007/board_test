
#include "battery.h"
#include "usart.h"
#include "delay.h"

//==========================================
//��ȡ�ֽ�
//==========================================
u16 Battery_Read_Word(u8 command)
{
	u8 i = 0;
	u8 read_buf[3];  	//�ݴ��ȡ������
	u8 crc = 0x00;   	//crcУ��ֵ
	u16 word;					//����������
	
	//��ʼ��ȡ
	I2C_Start();                				//��ʼ�ź�
	I2C_Write(SlaveAddress);    	//�����豸��ַ+д�źţ�+0��
	I2C_Write(command);    				 	//���ʹ洢��Ԫ��ַ
	I2C_Start();                				//��ʼ�ź�
	I2C_Write(SlaveAddress+1); 	//�����豸��ַ+���źţ�+1��	
	for(i = 0;i < 3;i++)
	{
		read_buf[i] = I2C_Read();
		if(i == 2)
			I2C_SendACK(1);			
		else
			I2C_SendACK(0);
	}
	I2C_Stop();			//��ȡ���
	
	delay_ms(2);
	//word = ((u16)read_buf[1]<<8) + read_buf[0];
	word = read_buf[1]*0xff + read_buf[0];
	//printf("value:%d, %d, %d\r\n", read_buf[0], read_buf[1], read_buf[2]);
	
	//crc У��
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
//��ȡ����
//==========================================
u8 Battery_Read_Halfword(u8 command)
{

	u8 i = 0;
	u8 read_buf[3];  	//�ݴ��ȡ������
	u8 crc = 0x00;   	//crcУ��ֵ
	u16 word;					//����������
	
	//��ʼ��ȡ
	I2C_Start();                				//��ʼ�ź�
	I2C_Write(SlaveAddress);    	//�����豸��ַ+д�źţ�+0��
	I2C_Write(command);    				 	//���ʹ洢��Ԫ��ַ
	I2C_Start();                				//��ʼ�ź�
	I2C_Write(SlaveAddress+1); 	//�����豸��ַ+���źţ�+1��	
	for(i = 0;i < 3;i++)
	{
		read_buf[i] = I2C_Read();
		if(i == 2)
			I2C_SendACK(1);			
		else
			I2C_SendACK(0);
	}
	I2C_Stop();			//��ȡ���
	
	delay_ms(1);

	//printf("value:%d\r\n", halfword);
	
	return read_buf[0];
}


//==========================================
//��ȡ����¶�  ��λ�����϶�
//�����󣬷���-1
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
//��ȡ��ص�ѹ ��λ��v
//�����󣬷���-1
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
//��ȡ��ص���  ��λ��a
//�����󣬷���-1
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
//��ȡ���ƽ������  ��λ��a
//�����󣬷���-1
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
//��ȡ�����������ٷֱ�  ��λ��%
//�����󣬷���-1
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
//��ȡ��ؾ��������ٷֱ�  ��λ��%
//�����󣬷���-1
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
//�����ز���
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
	
