#include "BH1750.h"


u8    BH_BUF[8];                   //接收数据缓存区


void  Single_Write_BH1750(uchar REG_Address);               //单个写入数据
uchar Single_Read_BH1750(uchar REG_Address);                //单个读取内部寄存器数据
void  Multiple_Read_BH1750();                               //连续的读取内部寄存器数据
//------------------------------------
void BH1750_Start();                    //起始信号
void BH1750_Stop();                     //停止信号
void BH1750_SendACK(bit ack);           //应答ACK
bit  BH1750_RecvACK();                  //读ack
void BH1750_SendByte(BYTE dat);         //IIC单个字节写
BYTE BH1750_RecvByte();                 //IIC单个字节读


/**************************************
起始信号
**************************************/
void BH1750_Start()
{
	SDA = 1;                    //拉高数据线
	SCL = 1;                    //拉高时钟线
	delay5us();                 //延时
	SDA = 0;                    //产生下降沿
	delay5us();                 //延时
	SCL = 0;                    //拉低时钟线
}

/**************************************
停止信号
**************************************/
void BH1750_Stop()
{
	SDA = 0;                    //拉低数据线
	SCL = 1;                    //拉高时钟线
	delay5us();                 //延时
	SDA = 1;                    //产生上升沿
	delay5us();                 //延时
}

/**************************************
发送应答信号
入口参数:ack (0:ACK 1:NAK)
**************************************/
void BH1750_SendACK(bit ack)
{
	SDA = ack;                  //写应答信号
	SCL = 1;                    //拉高时钟线
	delay5us();                 //延时
	SCL = 0;                    //拉低时钟线
	delay5us();                 //延时
}

/**************************************
接收应答信号
**************************************/
bit BH1750_RecvACK()
{
	SCL = 1;                    //拉高时钟线
	delay5us();                 //延时
	CY = SDA;                   //读应答信号
	SCL = 0;                    //拉低时钟线
	delay5us();                 //延时

	return CY;
}

/**************************************
向IIC总线发送一个字节数据
**************************************/
void BH1750_SendByte(BYTE dat)
{
	BYTE i;

	for (i=0; i<8; i++)         //8位计数器
	{
		dat <<= 1;              //移出数据的最高位
		SDA = CY;               //送数据口
		SCL = 1;                //拉高时钟线
		delay5us();             //延时
		SCL = 0;                //拉低时钟线
		delay5us();             //延时
	}
	BH1750_RecvACK();
}

/**************************************
从IIC总线接收一个字节数据
**************************************/
BYTE BH1750_RecvByte()
{
	BYTE i;
	BYTE dat = 0;

	SDA = 1;                    //使能内部上拉,准备读取数据,
	for (i=0; i<8; i++)         //8位计数器
	{
		dat <<= 1;
		SCL = 1;                //拉高时钟线
		delay5us();             //延时
		dat |= SDA;             //读数据
		SCL = 0;                //拉低时钟线
		delay5us();             //延时
	}
	return dat;
}

//*********************************

void Single_Write_BH1750(uchar REG_Address)
{
	BH1750_Start();                  //起始信号
	BH1750_SendByte(SlaveAddress);   //发送设备地址+写信号
	BH1750_SendByte(REG_Address);    //内部寄存器地址，请参考中文pdf22页
	//  BH1750_SendByte(REG_data);       //内部寄存器数据，请参考中文pdf22页
	BH1750_Stop();                   //发送停止信号
}

//********单字节读取*****************************************
/*
uchar Single_Read_BH1750(uchar REG_Address)
{  uchar REG_data;
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress);           //发送设备地址+写信号
    BH1750_SendByte(REG_Address);                   //发送存储单元地址，从0开始
    BH1750_Start();                          //起始信号
    BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号
    REG_data=BH1750_RecvByte();              //读出寄存器数据
	BH1750_SendACK(1);
	BH1750_Stop();                           //停止信号
    return REG_data;
}
*/
//*********************************************************
//
//连续读出BH1750内部数据
//
//*********************************************************
float Multiple_read_BH1750(void)
{
	uchar i;
	int temp;
	float dat;
	
	Single_Write_BH1750(0x01);   // power on
	Single_Write_BH1750(0x10);   // H- resolution mode
	delay_ms(180);              //延时180ms
	
	
	BH1750_Start();                          //起始信号
	BH1750_SendByte(SlaveAddress+1);         //发送设备地址+读信号

	for (i=0; i<3; i++)                      //连续读取6个地址数据，存储中BUF
	{
		BH_BUF[i] = BH1750_RecvByte();          //BUF[0]存储0x32地址中的数据
		if (i == 3)
		{

			BH1750_SendACK(1);                //最后一个数据需要回NOACK
		}
		else
		{
			BH1750_SendACK(0);                //回应ACK
		}
	}

	BH1750_Stop();                          //停止信号
	
	temp=BH_BUF[0];
	temp=(temp<<8)+BH_BUF[1];//合成数据
	dat = (float)temp/1.2;
	delay_ms(5);
	
	return dat;
}


//初始化BH1750，根据需要请参考pdf进行修改****
void Init_BH1750(void)
{
	Single_Write_BH1750(0x01);
}