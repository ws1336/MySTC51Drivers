#ifndef __NEC_IR_CODING_H__
#define __NEC_IR_CODING_H__
#include"define.h"
#include"DELAY.h"
/**************用红外二极管实现NEC格式红外编码驱动*****************/

sbit IR_Send=P3^2; //红外led接口

//本函数占用定时器0和定时器1

extern void NEC_IR_send_order(uchar a,b,c,d);//4字节指令
#endif