#ifndef __BH1750_H__
#define __BH1750_H__
#include "define.h"
#include "delay.h"
#include  <math.h>    //Keil library  
#include  <stdio.h>   //Keil library	
#include  <INTRINS.H>

sbit	  SCL=P1^0;      //IIC时钟引脚定义
sbit  	  SDA=P1^1;      //IIC数据引脚定义

#define	  SlaveAddress   0x46 	//定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改

void Init_BH1750(void); 			//初始化
float Multiple_read_BH1750(void); 	//返回值：光强 单位：lx

#endif