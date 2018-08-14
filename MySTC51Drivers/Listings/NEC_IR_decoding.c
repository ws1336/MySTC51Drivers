#include "NEC_IR_decoding.h"

struct IR_STA IR_Sta;
/*-----------函数声明------------*/
void translate();
/*---------------解码----------------*/
u8 NEC_IR_decoding(u8* num)
{
	u8 i;   
		if(IR_Sta.Receive_ok)
		{
			translate();
			for(i=0;i<4;i++)
				num[i]=IR_Sta.Number[i];
			return 0;
		}
		return 1;
}
/*********初始化***************/
#if Fosc==11059200UL
void IR_init()
{	
	AUXR &= 0x7F;		//定时器时钟12T模式	
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x02;		//设置定时器模式
	TL0 = 0xA4;			//设置定时初值
	TH0 = 0xA4;			//设置定时重载值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	EA = 1;		 		//打开总中断
	ET0 = 1;   	 		//打开定时器中断0
	EX0 = 1;			//打开外部中断0
	IT0 = 1;				//外部中断0下降沿触发
}
#elif Fosc==12000000UL
void IR_init()
{	
	AUXR &= 0x7F;		//定时器时钟12T模式	
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x02;		//设置定时器模式
	TL0 = 0x9C;			//设置定时初值
	TH0 = 0x9C;			//设置定时重载值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	EA = 1;		 		//打开总中断
	ET0 = 1;   	 		//打开定时器中断0
#if IR_Receive_Int==0
	EX0 = 1;			//打开外部中断0
	IT0 = 1;			//外部中断0下降沿触发
#elif IR_Receive_Int==1
	EX1 = 1;			//打开外部中断1
	IT1 = 1;			//外部中断1下降沿触发
#endif	
	IR_Sta.Counter=0;
    IR_Sta.Receive_ok=0;
    IR_Sta.Start_flag=0;
    IR_Sta.Now_flag=0;
    
}
#endif
/***********定时器中断************/
void timer0() interrupt 1
{
	IR_Sta.Counter++;		   //	100us
}

/*------红外接收中断------*/
#if IR_Receive_Int==0 
void IR_read() interrupt 0 
#elif IR_Receive_Int==1 
void IR_read() interrupt 2 
#endif
{
	if(IR_Sta.Start_flag)
	{
		if(IR_Sta.Counter>IR_DATA_S_L&&IR_Sta.Counter<IR_DATA_S_H)
		{
			IR_Sta.Dat[0] = 1;
			IR_Sta.Now_flag=1;
		}
		else if(IR_Sta.Dat[0] == 1)
		{	
			if(IR_Sta.Counter>IR_DATA_0_L&&IR_Sta.Counter<IR_DATA_0_H)
				IR_Sta.Dat[IR_Sta.Now_flag] = 0;
			else if(IR_Sta.Counter>IR_DATA_1_L&&IR_Sta.Counter<IR_DATA_1_H) 
				IR_Sta.Dat[IR_Sta.Now_flag] = 1;
			else
				{ 
					IR_Sta.Start_flag = 1;
					IR_Sta.Dat[0] = 0;
					IR_Sta.Now_flag=1;										 
				}
			IR_Sta.Now_flag++;	  
			if(IR_Sta.Now_flag==33)
			{
				IR_Sta.Dat[0] = 0;
				IR_Sta.Receive_ok = 1;
				IR_Sta.Start_flag = 0;
				IR_Sta.Now_flag = 1;
			}			
		} 
		else
		{
			IR_Sta.Start_flag = 1;
			IR_Sta.Dat[0] = 1;
		}
	}
	else
	{
		IR_Sta.Start_flag = 1;
	}
	IR_Sta.Counter = 0;			
}
/*----------译码---------*/
void translate()
{
	uchar i,j;
	for(i=0;i<4;i++)
		for(j=0;j<8;j++)
			{ 	
				IR_Sta.Number[i]>>=1;
				if(IR_Sta.Dat[i*8+j+1])
				IR_Sta.Number[i] |= 0x80;
			}
	IR_Sta.Receive_ok = 0;	
}
