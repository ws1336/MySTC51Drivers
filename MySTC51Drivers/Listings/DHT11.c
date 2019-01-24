#include "DHT11.h"



u8 COM(u8 * U8comdata)
{
	u8 i;
	u8 U8FLAG,U8temp;
	for(i=0; i<8; i++)
	{

		U8FLAG=2;
		while((!P2_0)&&U8FLAG++);
		delay10us();
		delay10us();

		delay10us();
		U8temp=0;
		if(P2_0)U8temp=1;
		U8FLAG=2;
		while((P2_0)&&U8FLAG++);
		//超时则跳出for循环
		if(U8FLAG==1) return 0;
		//判断数据位是0还是1

		// 如果高电平高过预定0高电平值则数据位为 1

		*U8comdata<<=1;
		*U8comdata|=U8temp;        //0
	}//rof
	return 1;
}


u8 DH11_dat[5];
u8 dht11_get_data(u8 *RH, u8 *T)
{
	u8 U8FLAG,U8temp;
	
	//主机拉低18ms
	P2_0=0;
	delay_ms(20);
	P2_0=1;
	//总线由上拉电阻拉高 主机延时20us
	delay10us();
	delay10us();
	delay10us();
	delay10us();
	//主机设为输入 判断从机响应信号
	P2_0=1;
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行
	if(!P2_0)		 //T !
	{
		U8FLAG=2;
		//判断从机是否发出 80us 的低电平响应信号是否结束
		while((!P2_0)&&U8FLAG++);
		U8FLAG=2;
		//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		while((P2_0)&&U8FLAG++);
		//数据接收状态
		if(!COM(&(DH11_dat[0]))) return 1;
		if(!COM(&(DH11_dat[1]))) return 2;
		if(!COM(&(DH11_dat[2]))) return 3;
		if(!COM(&(DH11_dat[3]))) return 4;
		if(!COM(&(DH11_dat[4]))) return 5;
		P2_0=1;
		//数据校验

		U8temp=(DH11_dat[0]+DH11_dat[1]+DH11_dat[2]+DH11_dat[3]);
		if(U8temp==DH11_dat[4])
		{
			*RH = DH11_dat[0];
			*T = DH11_dat[2];
			return 0;
		}//fi
	}//fi
	return 6;
}
