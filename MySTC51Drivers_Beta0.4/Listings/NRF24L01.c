#include"define.h"
#include"DELAY.h"
#include"NRF24L01.h"

uchar code TX_ADDRESS[5] = {0,0,0,0,1}; 	//本机地址
uchar code RX_ADDRESS_P0[5] = {0,0,0,0,1};//从机0地址
uchar code RX_ADDRESS_P1[5] = {1,1,1,1,1};//从机1地址
uchar code RX_ADDRESS_P2[1] = {2};        //从机2地址
uchar code RX_ADDRESS_P3[1] = {3};        //从机3地址
uchar code RX_ADDRESS_P4[1] = {4};        //从机4地址
uchar code RX_ADDRESS_P5[1] = {5};        //从机5地址
#define Data_width  1 //数据长度
#define Data_rt  15   //自动重发次数

/**/
uchar SPI_RW(uchar dat)		 //	SPI读写指令
{
	uchar i;
	for(i=0;i<8;i++)
	{
		SCK = 0;
		MOSI = (dat&0x80);
		dat <<= 1;
		SCK = 1;
		dat |= MISO; 
	}
	SCK = 0;
	return dat;
}
uchar NRF24L01_read_reg(uchar reg)	//读某个寄存器的状态
{
	uchar value;
	CSN=0;					 //拉低CSN，允许操作
  	SPI_RW(reg);	 		 //写寄存器指令
	value = SPI_RW(0);		 //读寄存器值
	CSN=1;					 //拉高CSN，禁止操作
	return value;			 //返回寄存器状态
}
uchar NRF24L01_write_reg(uchar reg,uchar value)	  //写向某个寄存器写指令，并读出状态
{
	uchar status;
	CSN=0;					//拉低CSN，允许操作
  	status = SPI_RW(reg);	//写寄存器指令，并读出寄存器状态
	SPI_RW(value);			//写寄存器值
	CSN=1;					//拉高CSN，禁止操作
	return status;			//返回寄存器之前的值
}
uchar NRF24L01_read_buf(uchar reg,uchar *pbuf,uchar n)
{
   	uchar i,status;
   	CSN=0;					//拉低CSN，允许操作
   	status = SPI_RW(reg);	//写寄存器指令，并读出寄存器状态
	for(i=0;i<n;i++)
		pbuf[i] = SPI_RW(0);//从寄存器读出一字节数据
	CSN = 1;				//拉高CSN，禁止操作
	return status;
}
uchar NRF24L01_write_buf(uchar reg,uchar *pbuf,uchar n)
{
   	uchar i,status;
   	CSN=0;					//拉低CSN，允许操作
   	status = SPI_RW(reg);	//写寄存器指令，并读出寄存器状态
	for(i=0;i<n;i++)
		SPI_RW(pbuf[i]);	//写一字节数据到寄存器
	CSN = 1;				//拉高CSN，禁止操作
	return status;
} 

void NRF24L01_init()
{
	CE = 0;		   //射频电路工作使能，高电平工作，低电平停止
	CSN = 1;	   //SPI操作高电平允许，低电平禁止
	SCK = 0;	   //时钟拉低，禁止读写寄存器
	IRQ = 1;	   //中断复位，等待产生中断信号
	NRF24L01_write_reg(WRITE_REG + EN_AA, 0x3f);	 			//所有接受通道允许自动应答
	NRF24L01_write_reg(WRITE_REG + EN_RXADDR, 0x3f);	 		//接收通道全部打开
	NRF24L01_write_reg(WRITE_REG + SETUP_AW, 0x03);	 			//设置接收/发射地址宽度为5字节
	NRF24L01_write_reg(WRITE_REG + SETUP_RETR, Data_rt);		//自动发送间隔250+86us，次数15
	NRF24L01_write_reg(WRITE_REG + RF_CH, 0x00);		 		//设置信道工作为2.4Ghz，收发必须一致
	NRF24L01_write_reg(WRITE_REG + RX_PW_P0, Data_width);		//设置通道0数据字节数
	NRF24L01_write_reg(WRITE_REG + RX_PW_P1, Data_width);		//设置通道1数据字节数
	NRF24L01_write_reg(WRITE_REG + RX_PW_P2, Data_width);		//设置通道2数据字节数
	NRF24L01_write_reg(WRITE_REG + RX_PW_P3, Data_width);		//设置通道3数据字节数
	NRF24L01_write_reg(WRITE_REG + RX_PW_P4, Data_width);		//设置通道4数据字节数
	NRF24L01_write_reg(WRITE_REG + RX_PW_P5, Data_width);		//设置通道5数据字节数
	NRF24L01_write_reg(WRITE_REG + RF_SETUP, 0x0f);	 			//发送速率为1Mhz，发送功率最大值0db
	NRF24L01_write_buf(WRITE_REG + TX_ADDR,TX_ADDRESS,5);		//写本机地地址
	NRF24L01_write_buf(WRITE_REG + RX_ADDR_P0,RX_ADDRESS_P0,5);	//写数据通道0接收机地址
	NRF24L01_write_buf(WRITE_REG + RX_ADDR_P1,RX_ADDRESS_P1,5);	//写数据通道1接收机地址
	NRF24L01_write_buf(WRITE_REG + RX_ADDR_P2,RX_ADDRESS_P2,1); //写数据通道2接收机地址
	NRF24L01_write_buf(WRITE_REG + RX_ADDR_P3,RX_ADDRESS_P3,1); //写数据通道3接收机地址
	NRF24L01_write_buf(WRITE_REG + RX_ADDR_P4,RX_ADDRESS_P4,1); //写数据通道4接收机地址
	NRF24L01_write_buf(WRITE_REG + RX_ADDR_P5,RX_ADDRESS_P5,1); //写数据通道5接收机地址
} 
void RX_MODE()
{
	NRF24L01_write_reg(WRITE_REG + CONFIG, 0x0f);//IRQ收发完成中断响应，16位CRC，接收模式  
	CE = 1;
}
void TX_MODE()
{
	NRF24L01_write_reg(WRITE_REG + CONFIG, 0x0e);//IRQ收发完成中断响应，16位CRC，发送模式
	CE = 1;
}
uchar TX_packet(uchar *tx_buf)
{
	uchar tx_flag = 1;
 	CE = 0;	//停止射频电路工作
	NRF24L01_write_reg(WRITE_REG + STATUS,0xff);			//清除中断标志位	
	NRF24L01_write_buf(WR_TX_PLOAD,tx_buf,Data_width);		//装载要发送的数据
	CE = 1;	//置高CE，激发数据发送
	delay_ms(Data_rt/2);   
	if(NRF24L01_read_reg(STATUS)&0x10)
		tx_flag = 0;
	return(tx_flag);	 
}
uchar RX_packet(uchar *rx_buf)
{
   	uchar revalue = 0,sta;
	sta = NRF24L01_read_reg(STATUS);	  	//读状态寄存器
	if(sta&0x40) 	//如果接受中断标志位为1
	{
		CE = 0;										   	//SPI使能
	 	NRF24L01_read_buf(RD_RX_PLOAD,rx_buf,Data_width);	//读取数据
		revalue = 1;								   	//读取数据完成标志置1	
		NRF24L01_write_reg(WRITE_REG + STATUS,0xff);	//清除中断标志位	
	}
	CE = 1;
	return revalue;//返回读取数据完成标志
}