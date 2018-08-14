#ifndef __DEFINE_H__
#define __DEFINE_H__
#include <STC12C5A60S2.H>   //51头文件
typedef unsigned short uint;
typedef unsigned short u16;
typedef unsigned short uint16;
typedef unsigned short int16u;
typedef unsigned long  int32u;
typedef unsigned long  uint32;
typedef unsigned long  u32;
typedef	unsigned char uchar;
typedef	unsigned char u8;
typedef	unsigned char uint8;
typedef	unsigned char int8u;
typedef unsigned char BYTE;
typedef unsigned short WORD;  

/******************设置项********************/
#define Fosc	11059200UL    //晶振频率（HZ）
#define Machine_Cycle	12UL   //机器周期 （89c52系列==12	   stc11/12/15系列==2）
#define AT24C02_ADDR  0xa0	//AT24C02地址
#define PCF8591_ADDR  0x90	//PCF8591地址
#endif
