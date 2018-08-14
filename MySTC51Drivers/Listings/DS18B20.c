#include"ds18b20.h"
void tempdelay(uchar i)	 //6.63us
{
	while(i--);
}
void reset()
{
	DS = 1;
	_nop_();
	DS = 0;
	tempdelay(80);
	DS = 1;
	tempdelay(40);
	DS = 1;
	_nop_();	
}
bit read_bit()
{
	bit temp;
	DS = 0;
	_nop_();
	_nop_();
	DS = 1;
	_nop_();
	temp = DS;
	tempdelay(12);
	return temp;
}
uchar read_byte()
{
	uchar temp,value,i=8;
	while(i--)
	{	
		temp=read_bit();
		value = (temp<<7)|(value>>1);
	}
	return value;
}
void write_byte(uchar dat)
{
	uchar i=8;
	while(i--)
	{
		DS = 0;
		_nop_();
		DS = dat&0x01;
		dat>>=1;
		tempdelay(11);
		DS = 1;
		_nop_();
	}	
}
void DS18B20_init()
{
  	reset();
	write_byte(0xcc);
	write_byte(0x4e);
	write_byte(0x7d);
	write_byte(0xc9);
	write_byte(0x7f);
}
void DS18B20_change()
{
	reset();
	write_byte(0xcc);
	write_byte(0x44);	
}
float DS18B20_read_temperature()
{
	float temperature;
	uchar l,h;
	int num;
	reset();
	write_byte(0xcc);
	write_byte(0xbe);
	l=read_byte();
	h=read_byte();
	num = h;
	num<<=8;
	num|=l;
	temperature = (float)num/16;
	return temperature;		
}