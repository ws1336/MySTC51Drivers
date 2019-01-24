#ifndef __DELAY_H__
#define __DELAY_H__
#include"define.h"
#include<intrins.h>

/****注意：请修改define.h中关于晶振频率和机器周期的定义****/
/****注意：请修改define.h中关于晶振频率和机器周期的定义****/
/****注意：请修改define.h中关于晶振频率和机器周期的定义****/

#define TIME_MS (Fosc/Machine_Cycle/1000)
#define TIME_I	((TIME_MS+506)/514)
#define TIME_J	(((TIME_MS+506)%514)/2)

/***********延时函数**************/
void delay_ms(uint ms);     //有误差，不过一般情况可以忽略，若系统中有中断则误差可能较大
void delay5us();
void delay10us();
#endif
