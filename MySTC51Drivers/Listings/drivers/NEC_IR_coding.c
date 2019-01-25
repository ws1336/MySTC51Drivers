#include "NEC_IR_coding.h" 
void send0()
{
	TR0 = 1;
	TH1 = (65536-516)/256;
	TL1 = (65536-516)%256;
	TR1 = 1;
	while(!TF1);
	TF1 = 0;
	TR1 = 0;

	TR0 = 0;
	IR_Send = 1;
	TH1 = (65536-516)/256;
	TL1 = (65536-516)%256;
	TR1 = 1;
	while(!TF1);
	TF1 = 0;
	TR1 = 0;
	IR_Send = 1;
}
void send1()
{
 	TR0 = 1;				 //打开红外发射定时器0 ，发射38khz红外
	TH1 = (65536-516)/256;
	TL1 = (65536-516)%256;	 //定时器T1定时560us
	TR1 = 1;				 //打开定时器1
	while(!TF1);			 //定时结束后
	TF1 = 0;				 //重置标志位
	TR1 = 0;				 //关闭定时器1

	TR0 = 0;
	IR_Send = 1;
	TH1 = (65536-1548)/256;	  ////定时器T1定时1650us
	TL1 = (65536-1548)%256;
	TR1 = 1;
	while(!TF1);
	TF1 = 0;
	TR1 = 0;
	IR_Send = 1;
}
void IRsend_byte(uchar num)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(num&0x01)
		send1();
		else
		send0();
		num>>=1;
	}
}
void IRstart()
{
	TR0 = 1;
	TH1 = (65536-8294)/256;
	TL1 = (65536-8294)%256;
	TR1 = 1;
	while(!TF1);
	TF1 = 0;
	TR1 = 0;

	TR0 = 0;
	IR_Send = 1;
	TH1 = (65536-4147)/256;
	TL1 = (65536-4147)%256;
	TR1 = 1;
	while(!TF1);
	TF1 = 0;
	TR1 = 0;
	IR_Send = 1;
}
void IRstop()
{
	TR0 = 1;
	TH1 = (65536-516)/256;
	TL1 = (65536-516)%256;
	TR1 = 1;
	while(!TF1);
	TF1 = 0;
	TR1 = 0;

	TR0 = 0;
	IR_Send = 1;
	TH1 = (65536-516)/256;
	TL1 = (65536-516 )%256;
	TR1 = 1;
	while(!TF1);
	TF1 = 0;
	TR1 = 0;
	IR_Send = 1;
}
void timer_init()
{
	TMOD = 0x12;
	TH0  = 0xf6;
	TL0  = 0xf6;
	EA   = 1;
	ET0  = 1;
}

void NEC_IR_send_order(u8 a,b,c,d)
{
	timer_init();
	delay_ms(5);
	IRstart();						 
	IRsend_byte(a);					 
	IRsend_byte(b);		
	IRsend_byte(c);
	IRsend_byte(d);
	IRstop();
}
void timer0()interrupt 1
{
	IR_Send = ~IR_Send;
}
