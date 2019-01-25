#ifndef __AD12C5A_H
#define __AD12C5A_H
#include "define.h"
#include "delay.h"

#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit


#define ADC_SPEEDLL 0x00            //420 clocks
#define ADC_SPEEDL  0x20            //280 clocks
#define ADC_SPEEDH  0x40            //140 clocks
#define ADC_SPEEDHH 0x60            //70 clocks     

//选择转换速度，如下
#define ADC_SPEED  ADC_SPEEDLL

#define ADC_0 0x01
#define ADC_1 0x02
#define ADC_2 0x04
#define ADC_3 0x08
#define ADC_4 0x10
#define ADC_5 0x20
#define ADC_6 0x40
#define ADC_7 0x80

//选择转换通道，P1口的某几个位，如下 
//#define ADC_IO (ADC_0|ADC_4)
#define ADC_IO (ADC_0|ADC_1|ADC_2|ADC_3|ADC_4|ADC_5|ADC_6|ADC_7)

void InitADC();
u8 GetADCResult(u8 ch);

#endif