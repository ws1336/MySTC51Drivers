#include "Digital_Tube.h"

unsigned char leddata[]={

                0x3F,  //"0"
                0x06,  //"1"
                0x5B,  //"2"
                0x4F,  //"3"
                0x66,  //"4"
                0x6D,  //"5"
                0x7D,  //"6"
                0x07,  //"7"
                0x7F,  //"8"
                0x6F,  //"9"
                0x77,  //"A"
                0x7C,  //"B"
                0x39,  //"C"
                0x5E,  //"D"
                0x79,  //"E"
                0x71,  //"F"
	              0x40   //"-"

};
//函数名：Digital_Display_One
//功能：		显示单个数码管
//参数：		Digit:要显示的数字
//				pos:  要显示的位置
//注意：		不同开发板只需要改写此函数，本文件其他数码管驱动即可使用
void  Digital_Display_One(u8 Digit,u8 pos)
{
		switch(pos)
		{
			case 0:Digital_Data = 0xfe;break;		//01111111
			case 1:Digital_Data = 0xfd;break; 	//10111111
			case 2:Digital_Data = 0xfb;break;		//11011111
			case 3:Digital_Data = 0xf7;break; 	//11101111
			case 4:Digital_Data = 0xef;break; 	//11110111
			case 5:Digital_Data = 0xdf;break;		//11111011
			case 6:Digital_Data = 0xbf;break; 	//11111101
			case 7:Digital_Data = 0x7f;break; 	//11111110
		}
		Digital_Wei = 1;
		Digital_Wei = 0;
		
		Digital_Data = leddata[Digit];
		Digital_Duan = 1;
		Digital_Duan = 0;
		
		delay_ms(1);
		
		Digital_Data = 0x00;	//关显示（消残影）	
		Digital_Duan = 1;
		Digital_Duan = 0;
	
}
void  Digital_display_time(u8 h,u8 m,u8 s)
{
	u8 i,time[8];
	time[7] = s%10;
	time[6] = s/10;
	time[5] = 16;
	time[4] = m%10;
	time[3] = m/10;
	time[2] = 16;
	time[1] = h%10;
	time[0] = h/10;
	for(i=0; i<8; i++)
	{				
		Digital_Display_One(time[i],i);	
	}
}
void  Digital_display_num(u32 num)
{
	u8 i;
	u8 n[8];
	n[0]=num/10000000%10;
	n[1]=num/1000000%10;
	n[2]=num/100000%10;
	n[3]=num/10000%10;
	n[4]=num/1000%10;
	n[5]=num/100%10;
	n[6]=num/10%10;
	n[7]=num/1%10;
	for(i=0; i<8; i++)
	{
		Digital_Display_One(n[i],i);
	}
}
void Digital_display_4hex(u8 *num)
{
	u8 n[8],i;
	n[0] = num[0]/16;
	n[1] = num[0]%16;
	n[2] = num[1]/16;
	n[3] = num[1]%16;
	n[4] = num[2]/16;
	n[5] = num[2]%16;
	n[6] = num[3]/16;
	n[7] = num[3]%16;

	for(i=0;i<8;i++)
	{
		Digital_Display_One(n[i],i);
	}		
}
