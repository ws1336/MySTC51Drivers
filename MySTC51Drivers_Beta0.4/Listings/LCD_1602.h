#ifndef __LCD_1602_H__
#define __LCD_1602_H__
#include "define.h"
#include <intrins.h>
#include <stdio.h>
#include "DELAY.h"
/*****************1602液晶屏驱动***************/

/*******************定义接口*************************/

#define LCD_1602_DATA P0

sbit LCD_1602_RS = P3^5;
sbit LCD_1602_RW = P3^6;
sbit LCD_1602_EN = P3^4;

/*******************驱动函数*************************/

void LCD_1602_init();		   		  									//初始化运行方式
void LCD_1602_clean();														//清屏
/**************** x=(1--4) y(1--8) p为字符串首地址 **/
void LCD_1602_display(u8 x,u8 y,u8 l,u8 *p);	//写字符串
#endif
