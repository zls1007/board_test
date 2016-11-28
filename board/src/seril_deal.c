/**
  ******************************************************************************
  * @file    seril_deal.c
  * @author  zhao linshan
  * @version V0.0.1
  * @date    2016-08-19
  * @brief   本文件处理串口接收到的串口指令（json）
	*			硬件资源：
  *          	对json字符串和对象进行解析和生成				
  *
	*			函数：
	*						SerilDeal(u8 *buf);  对串口数据进行处理
	*			1#  42   -2
	*			2#	42	 -2
	*			3#  40   -4
*/


#include "seril_deal.h"
#include "delay.h"

extern u8 seril4_buf[11];
extern float roll, pitch, yaw;

char send_buf[130];			//串口要发送的数据

//从综合管理板采集的状态
u16 arg1,arg2,arg3;
float deep;  	//深度
float cur;		//电流
float vol;		//电压
u8 IsWater;   //0-无漏水  1-漏水


/****************************************************************************
*			对接收到指令串进行处理
*			msg_type  -- 消息类型 move
*			result    -- 结果值 200
******************************************************************************/
void SerilDeal(u8 buf[])
{
	int arg[3];  //用来存储命令中的参数值
	u8 type = 0; //存储消息类型 0-未定义 1-运动 2-查询 3-照明
	u8 move_type = 0;  //用来存储运动类型 0-未定义 1-单通道 2-位姿控制
	u8 arg_num;
	u8 start;					//用来存储什么时候计数 0-复位 1-已找到标签 2-已找到数据开始位置 3-已确定正负号
	int flag; 				//正负号标记 
	int i, n;
	u8 c;
	int x,r,z;
	int d1, d2;
	int p1, p2, p3;
	int c1, c2, c3;
	
	//判断消息类型
	n = 2;
	for(i=0; buf[i] != '\n' && i<80; i++)		//遍历整个字符串
	{
		if(buf[i] == 'T')   //消息类型标签
		{
			c = buf[i+4];	
			if(c == 'm') {type = 1; n --;}
			else if(c == 'g') {type = 2; n = 0;}
			else if(c == 'l') {type = 3; n = 0;}
		}
		else if(buf[i] == 'M')  //运动类型
		{
			c = buf[i+4];
			if(c == 'c') move_type = 1;
			else if(c == 'v') move_type = 2;
			n --;
		}
		if(n == 0)		//取值结束，终止循环
			break;
	}
	
	//对不同类型消息进行解析
	if(type == 1 && move_type == 2)  //位姿控制
	{
		for(i=0; buf[i] != '\n' && i < 80; i++)  //遍历整个字符串
		{
			c = buf[i];
			if(c == 'x'){arg_num = 0; start = 1;}
			else if(c == 'r'){arg_num = 1; start = 1;}
			else if(c == 'z'){arg_num = 2; start = 1;}
			else if(c == ':' && start == 1){start = 2; arg[arg_num] = 0; flag = 1;}
			//else if(c == '-' && start == 2){start = 3; flag = -1;}
			else if(c == ',' && start == 2){start = 0; arg[arg_num] *= flag;}
			else if(c == '}' && start == 2){start = 0; arg[arg_num] *= flag;}
			else if(start == 2)
			{
				if(c == '-')
					flag = -1;
				else
					arg[arg_num] = arg[arg_num]*10 + c - '0';
			}
		}
		x = arg[0]; r = arg[1]; z = arg[2];
		printf("x=%d, r=%d, z=%d\r\n",x,r,z);
		
		//改变电机运动的速度

//		c1 = 1500 - x*5 - r*2 + 127;  //方向取反
//		c2 = 1500 + x*5 - r*2 + 24;
//		c3 = 1500 + z*2 + 24;
		
		//去掉死区
		if(x > 10) x -= 10;
		else if(x < -10) x += 10;
		else x = 0;
		if(r > 15) r -= 15;
		else if(r < -15) r += 15;
		else r = 0;
		if(z > 10) z -= 10;
		else if(z < -10) z += 10;
		else z = 0;
		
		//方向映射
		p1 = x*5 + r*3;
		p2 = -x*5 + r*3;
		p3 = z*3;
		
		if(p1 > 0) c1 = 1542 + p1;
		else if(p1 < 0) c1 = 1498 + p1;
		else c1 = 1522;
		if(p2 > 0) c2 = 1542 + p2;
		else if(p2 < 0) c2 = 1498 + p2;
		else c2 = 1522;
		if(p3 > 0) c3 = 1540 + p3;
		else if(p3 < 0) c3 = 1496 + p3;
		else c3 = 1522;
		
//		c1 = 1500 + x;
//		c2 = 1500 + r;
//		c3 = 1500 + z;
	
		if(c1 > 2000) c1 = 2000; 
		else if(c1 < 1000) c1 = 1000;
		if(c2 > 2000) c2 = 2000; 
		else if(c2 < 1000) c2 = 1000;
		if(c3 > 2000) c3 = 2000; 
		else if(c3 < 1000) c3 = 1000;
		TIM_SetCompare4(TIM1, c1);
		TIM_SetCompare3(TIM1, c2);
		TIM_SetCompare2(TIM1, c3);
		
		//发送指令结果
		SerilSendBack("move", 200);
	}
	else if(type == 2)  //查询指令
	{
		//发送机器人状态
		printf("get data\r\n");
		SerilSendState("get");
	}
	else if(type == 3)  //照明控制指令
	{
		for(i=0; buf[i] != '\n' && i < 80; i++)  //遍历整个字符串
		{
			c = buf[i];
			if(c == 'd'){if(buf[i+1]=='1') arg_num = 0; else if(buf[i+1] == '2') arg_num = 1; start = 1;}
			else if(c == ':' && start == 1){start = 2; arg[arg_num] = 0; flag = 1;}
			//else if(c == '-' && start == 2){start = 3; flag = -1;}
			else if(c == ',' && start == 2){start = 0; arg[arg_num] *= flag;}
			else if(c == '}' && start == 2){start = 0; arg[arg_num] *= flag;}
			else if(start == 2)
			{
				if(c == '-')
					flag = -1;
				else
					arg[arg_num] = arg[arg_num]*10 + c - '0';
			}
		}
		d1 = arg[0];  
		d2 = arg[1];
		printf("d1=%d,d2=%d\r\n", d1, d2);
		
		//发送指令结果
		SerilSendBack("led", 200);
		
		//向综合管理板发送数据
		Seril4_Send(d1, d2);
	}

	
}

/****************************************************************************
*			通过串口反馈指令处理结果
*			msg_type  -- 消息类型 move
*			result    -- 结果值 200
******************************************************************************/
void SerilSendBack(char *msg_type, u8 result)
{
	sprintf((char *)send_buf, "{\"T\":\"%s\",\"R\":%d}\r\n", msg_type, result);
	SendStr((char *)send_buf);  //通过串口发送出去
}

/****************************************************************************
*			通过串口反馈机器人的状态信息
*			msg_type  -- 消息类型 get 
*		
******************************************************************************/
void SerilSendState(char *msg_type)
{
	int16_t r, p, y;
	r = (int16_t)roll;
	p = (int16_t)pitch;
	y = (int16_t)yaw;
	
	sprintf((char *)send_buf, "{\"T\":\"%s\",\"V\":%.1f,\"C\":%.1f,\"D\":%.1f,\"L\":\"no\",\"S\":{\"r\":%d,\"p\":%d,\"y\":%d}}\r\n", msg_type, vol, cur, deep, r, p, y);
	//sprintf((char *)send_buf, "{\"T\":\"%s\",\"V\":11.8,\"C\":2.4,\"D\":1.5,\"L\":\"no\",\"S\":{\"r\":%d,\"p\":%d,\"y\":0}}\r\n", msg_type,  r, p);
	SendStr((char *)send_buf);  //通过串口发送出去
}


/****************************************************************************
*			对接收到综合管理板的数据进行处理

******************************************************************************/
void buf4_deal()
{
	
	arg1 = seril4_buf[3]*256 + seril4_buf[4];
	arg2 = seril4_buf[5]*256 + seril4_buf[6];
	arg3 = seril4_buf[7]*256 + seril4_buf[8];
	deep = arg1*3.3/0x0fff;
	cur = (arg2*3.3/0x0fff-2.66)*15;
	vol = arg3*3.3*4/0x0fff;
	IsWater = seril4_buf[9];
	
	deep -= 0.6;
	cur += 0.3;
	//printf("deep=%.1fm cur=%.1fA vol=%.1fV\r\n", deep, cur, vol);

	
}

/****************************************************************************
*			向综合管理板发送数据（控制led的亮度）

******************************************************************************/
void Seril4_Send(u8 d1, u8 d2)
{
	u8 i;
	u8 buf[8];
	buf[0] = 0xa5;
	buf[1] = 0x5a;
	buf[2] = 0xd1;
	buf[3] = d1;
	buf[4] = d2;
	buf[5] = 0x00;
	buf[6] = 0x00;
	buf[7] = 0xff;
	
	for(i=0; i<8; i++)
	{
		USART_SendData(USART3, buf[i]);
  	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	}
	
}

