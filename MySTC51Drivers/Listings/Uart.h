#ifndef __UART_H
#define __UART_H
#include "define.h"
#include "stdio.h"
/****注意：请修改define.h中关于晶振频率和机器周期的定义****/
/****注意：请修改define.h中关于晶振频率和机器周期的定义****/
/****注意：请修改define.h中关于晶振频率和机器周期的定义****/

/******************串口相关函数*********************************/
void uart_init();     //初始化串口----此函数只适用于晶振频率11.0592MHZ,如不相同自行修改源码
void print(u8 *str);  //输出字符串----用此函数配合digital_to_string也可实现输出数字
void putc(u8 ch);     //输出字符

//由于下方的函数占用1K左右Flash和40BYTEde左右RAM，酌情考虑是否使用，把下面的宏EN_PRINTF置为0则不编译
#define EN_PRINTF    		1       	//使能

//#define SIZE_OF_PRINTG	64          //输出缓冲区,弃用
void my_printf(const u8* str, ...);	    //格式化输出字符串----用法和标准C语言的printf函数相同 

#endif