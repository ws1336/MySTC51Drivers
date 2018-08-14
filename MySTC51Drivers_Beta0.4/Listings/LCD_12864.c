
#include "LCD_12864.h"


void rdbf(void)   
{ 
	while(1)    
	{    
		LCD_12864_RS=0;
	  LCD_12864_RW=1;
		LCD_12864_EN=0;    
		LCD_12864_DATA=0xFF;    
		LCD_12864_EN=1;    
		if(LCD_12864_BUSY == 0)    
		break;     
	} 
}
void wr_data(uchar wrdata)    
{   
	rdbf();  	//当BF=0时,模块内部方可接受新数据
	LCD_12864_RS=1;  
	LCD_12864_RW=0;  
	LCD_12864_EN=1;   
	LCD_12864_DATA=wrdata;   
	LCD_12864_EN=0; 
}  
void wr_cmd(uchar cmd)    
{        
	rdbf();     //当BF=0时,模块内部方可接受新指令  
	LCD_12864_RS=0;  
	LCD_12864_RW=0;  
	LCD_12864_EN=1;   
	LCD_12864_DATA=cmd;  
	LCD_12864_EN=0; 
}
void LCD_12864_display_string(uchar x,uchar y,uchar *seg) //x为行号,y为列号 
{   
	uchar i=0x80;  
	uint t=0;   
	switch(x)          //确定行  
	{   
		case 1: i=0x80; break;     //第一行   
		case 2: i=0x90; break;     //第二行  
		case 3: i=0x88; break;     //第三行
		case 4: i=0x98; break;     //第四行 
		default : break;  
	}   
	i=i+y-1;     //确定列号 
	wr_cmd(i);   
	while(seg[t]!='\0')  
	{   
		wr_data(seg[t]); //写入需要显示字符的显示码   
		t++;  
	} 
}
void LCD_12864_init(void)    
{   
	LCD_12864_PSB = 1; //指定并行接口	     
	wr_cmd(0x30);	//功能设定：基本指令集
//	wr_cmd(0x36);	//功能设定：扩充指令集
	   
	wr_cmd(0x30);   //二次设定 
	   
	wr_cmd(0x01);    //清屏   

	wr_cmd(0x02);    //地址归位   
	   
	wr_cmd(0x06);   //光标右移，整体显示不移动 
//	wr_cmd(0x07);   //光标右移，整体显示随光标移动
//	wr_cmd(0x04);   //光标左移，整体显示不移动 
//	wr_cmd(0x05);   //光标左移，整体显示随光标移动    
	
//	wr_cmd(0x08);   //整体显示关，光标关，游标位置不反白   
	wr_cmd(0x0c);   //整体显示开，光标关，游标位置不反白
//	wr_cmd(0x0e);   //整体显示开，光标开，游标位置不反白
//	wr_cmd(0x0f);   //整体显示开，光标开，游标位置反白  
}   