#ifndef __QMC5883_H__
#define __QMC5883_H__
#include "define.h"	
#include <math.h>     
#include <stdio.h>   
#include <INTRINS.H>
#include "delay.h"
#include "I2C.h"

#define QMC5883_ADDR               0x1a 

/***********函数**************/
u8 QMC5883_InitConfig(void);            	//初始化5883
u8 QMC5883_GetData(float *angle);           //与北方向夹角
#endif
