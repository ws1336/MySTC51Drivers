#include "DELAY.h"

void delay_ms(uint ms)  
{	
	 unsigned char i, j;
 	do
	{		    
		i = TIME_I;
		j = TIME_J;
		do
		{
			while (--j);
		} while (--i);	
	}while(--ms);
}

#if Machine_Cycle==12UL
void delay5us()		    //@12.000MHz
{
	_nop_();
}
void delay10us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	i = 2;
	while (--i);
}
#elif Machine_Cycle==2UL
void delay5us()		    //@12.000MHz
{
	unsigned char i;

	i = 12;
	while (--i);
}
void delay10us()		//@12.000MHz
{
	unsigned char i;

	i = 27;
	while (--i);
}
#endif

