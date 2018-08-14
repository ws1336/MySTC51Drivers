#include "uart.h"

#if Fosc==11059200     //如果定义了晶振频率11059200
	#if Machine_Cycle==2UL   //12c5a60s2
		void uart_init()     //波特率19200
		{
			PCON |= 0x80;		//使能波特率倍速位SMOD
			SCON = 0x50;		//8位数据,可变波特率
			AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
			AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
			TMOD &= 0x0F;		//清除定时器1模式位
			TMOD |= 0x20;		//设定定时器1为8位自动重装方式
			TL1 = 0xDC;			//设定定时初值
			TH1 = 0xDC;			//设定定时器重装值
			ET1 = 0;				//禁止定时器1中断
			TR1 = 1;				//启动定时器1
		}

	#elif Machine_Cycle==12UL    //89c52
		void uart_init()     //19200
		{
			PCON |= 0x80;		//使能波特率倍速位SMOD
			SCON = 0x50;		//8位数据,可变波特率
			AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
			AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
			TMOD &= 0x0F;		//清除定时器1模式位
			TMOD |= 0x20;		//设定定时器1为8位自动重装方式
			TL1 = 0xFD;			//设定定时初值
			TH1 = 0xFD;			//设定定时器重装值
			ET1 = 0;				//禁止定时器1中断
			TR1 = 1;				//启动定时器1
		}
	#endif
#elif Fosc==12000000     //如果定义了晶振频率12000000
	#if Machine_Cycle==2UL   //12c5a60s2
		void uart_init()     //波特率19200
		{
			PCON |= 0x80;		//使能波特率倍速位SMOD
			SCON = 0x50;		//8位数据,可变波特率
			AUXR |= 0x40;		//定时器1时钟为Fosc,即1T
			AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
			TMOD &= 0x0F;		//清除定时器1模式位
			TMOD |= 0x20;		//设定定时器1为8位自动重装方式
			TL1 = 0xD9;		//设定定时初值
			TH1 = 0xD9;		//设定定时器重装值
			ET1 = 0;		//禁止定时器1中断
			TR1 = 1;		//启动定时器1
		}

	#elif Machine_Cycle==12UL    //89c52
		void uart_init()     //4800
		{
			PCON |= 0x80;		//使能波特率倍速位SMOD
			SCON = 0x50;		//8位数据,可变波特率
			AUXR &= 0xBF;		//定时器1时钟为Fosc/12,即12T
			AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
			TMOD &= 0x0F;		//清除定时器1模式位
			TMOD |= 0x20;		//设定定时器1为8位自动重装方式
			TL1 = 0xF3;		//设定定时初值
			TH1 = 0xF3;		//设定定时器重装值
			ET1 = 0;		//禁止定时器1中断
			TR1 = 1;		//启动定时器1
		}
	#endif
#endif
void putc(u8 ch)
{
	TI = 0;
	SBUF = ch;	        
	while(!TI);			//等待发送完成
	TI = 0;	
}
void print(u8* str)
{
	while(*str!='\0')
	{
		putc(*str);
		str++;
	}
} 

#if EN_PRINTF
/*
void my_printf(u8* str, ...)
{
	u8 string[SIZE_OF_PRINTG];
  void* Arg=0;
	Arg=(u8 *)&str;
  Arg=(u8 *)Arg+sizeof(str); //堆栈4字节对齐	
	vsprintf(string,str,Arg);
	print(string);
	Arg=0;
	
} 
*/
void my_printf(u8* str, ...)
{
  void* Arg=(u8 *)&str+sizeof(str);
	TI = 1;
	vprintf(str,Arg);
	Arg=0;
	
}
#endif
