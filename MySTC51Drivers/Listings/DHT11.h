#ifndef __DHT11_H__
#define __DHT11_H__

#include "define.h"
#include "delay.h"

//IO定义
sbit P2_0=P2^0;

//返回0 成功
//返回其他值 错误代码：（定位用）
//参数：湿度，温度
u8 dht11_get_data(u8 *RH,u8 *T);
#endif