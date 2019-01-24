#include<reg52.h>
#include"define.h"
#include"DELAY.h"
#include"step_motor.h"
uchar code Step_motor_table[]={0xf1,0xf3,0xf2,0xf6,0xf4,0xfc,0xf8,0xf9};
uchar code Step_motor_table1[]={0xf9,0xf8,0xfc,0xf4,0xf6,0xf2,0xf3,0xf1};
void Step_motor(bit Direction)			 //循环512次转一圈
{	
	uchar i;
	if(Direction)
	for(i=0;i<8;i++)
	{
		P1=Step_motor_table[i];
		Delay_ms(20);
	}
	else
	for(i=0;i<8;i++)
	{
		P1=Step_motor_table1[i];
		Delay_ms(20);
	}
}