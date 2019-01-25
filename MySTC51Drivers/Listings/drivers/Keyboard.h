#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include"define.h"
#include"delay.h"
/**********矩阵键盘驱动**********/
#define KEY_BOARD_PORT P3        //矩阵键盘接口
#define INDEPEND_KEY_PORT P3     //独立键盘接口
uchar Key_read();		//返回编码 (1--20)  无消抖，支持连按

#endif
