#include"I2C.h"

/*I2C初始化*/
void I2C_init()
{
	SDA = 1;
	_nop_();
	SCL = 1;
	_nop_();
}
/*I2C起始信号*/
void I2C_start()
{
	SCL = 1;
	delay5us();
	SDA = 0;
	delay5us();
	SCL = 0;
	_nop_();
	SDA	= 1;
	_nop_();

}
/*I2C终止信号*/
void I2C_stop()
{
	SDA = 0;
	_nop_();
	SCL = 1;
	delay5us();
	SDA = 1;
	delay5us();
}
/*主机发送应答*/
void Send_ACK(bit i)
{
	if(i)
		SDA = 0;
	else
		SDA = 1;
		_nop_();
		SCL = 1;
		delay5us();
		SCL = 0;
		_nop_();
		SDA = 1;
		_nop_();
}
/*检测从机应答*/
bit Test_ACK()
{
	bit ack;
	SCL = 1;
	delay5us();
	if(SDA)
	ack = 0;
	else
	ack = 1;
	SCL = 0;
	_nop_();
	return ack;
}
/*发送数据*/
void Write_byte(uchar dat)
{
	uchar i;
	bit temp;
	for(i=0;i<8;i++)
	{
		SCL = 0;
		_nop_();
		if(dat&0x80)
			temp = 1;	  //1000 0000
		else
			temp = 0;
		SDA = temp;
		_nop_();
		SCL = 1;
		_nop_();

		dat<<=1;
	}
	SCL = 0;
	_nop_();
	SDA = 1;
	_nop_();
}
/*接收数据*/
uchar Read_byte()
{
	uchar key = 0,i;
	for(i=0;i<8;i++)
	{
		SCL = 1;
		delay5us();
		key<<=1;
		if(SDA)
			key |= 0x01;
		else
		key &= 0xfe;
		SCL = 0;
		_nop_();
	}
	return key;
}

uchar I2C_write_onedata(uchar Device_address,uchar word_address,uchar dat)
{
	I2C_init();
	I2C_start();
	Write_byte(Device_address);
	if(!Test_ACK())
	return 2;
	Write_byte(word_address);
	if(!Test_ACK())
	return 1;
	Write_byte(dat);
	Send_ACK(0);
	I2C_stop();
	return 0;
}
uchar I2C_read_onedata(uchar Device_address,uchar word_address,uchar *dat)
{
	I2C_init();
	I2C_start();
	Write_byte(Device_address);
	if(!Test_ACK())
	return 3;
	Write_byte(word_address);
	if(!Test_ACK())
	return 2;
	I2C_start();
	Write_byte(Device_address+1);
	if(!Test_ACK())
	return 1;
	*dat = Read_byte();
	Send_ACK(0);
	I2C_stop();
	return 0;
}
uchar I2C_read_data(uchar Device_address,uchar word_address,uchar *dat,uint n)
{
	uchar t2,i;
	for(i=0;i<n;i++)
	{
		t2=I2C_read_onedata(Device_address,word_address+i,&dat[i]);
		delay_ms(2);
	}
	return t2;

}
uchar I2C_write_data(uchar Device_address,uchar word_address,uchar *dat,uint n)
{
	uchar t1,i;
	for(i=0;i<n;i++)
	{
		t1=I2C_write_onedata(Device_address,word_address+i,dat[i]);
		delay_ms(2);
	}
	return t1;
}
