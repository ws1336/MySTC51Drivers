#ifndef __NEC_IR_DECODING_H__
#define __NEC_IR_DECODING_H__
#include"define.h"

/******************** NEC格式红外解码驱动***************************/

/********************本驱动占用定时器0和外部中断0*******************/

#define IR_DATA_S_L  90        //引导码最短时长阈值
#define IR_DATA_S_H  180       //引导码最长时长阈值
#define IR_DATA_0_L  6         //数据0最短时长阈值
#define IR_DATA_0_H  16        //数据0最长时长阈值
#define IR_DATA_1_L  17        //数据1最短时长阈值
#define IR_DATA_1_H  27        //数据1最长时长阈值

#define IR_Receive_Int 0 //一体化接收头接口，外部中断0
//#define IR_Receive_Int 1 //一体化接收头接口，外部中断1

struct IR_STA{
	u8 Number[4];     //解码后的数据
	u8 Dat[33];       //原始数据
	u16 Counter;      //计数器
	u8  Receive_ok;   //接收完成
	u8  Start_flag;   //开始接受
	u8  Now_flag;     //1为开始
};

//变量名：IR_Sta
//变量功能：状态标记，只读，读此结构体变量的成员可获取解码过程信息，如是否正在接收红外码
extern struct IR_STA IR_Sta; 
//函数名：IR_init
//函数功能：红外解码系统初始化
void IR_init();  			//本函数占用定时器0和外部中断0
//函数名：NEC_IR_decoding
//函数功能：红外解码
//形参：num:为指向存放4个数据的指针 
//返回值： 为0解码成功		为1解码失败
u8 NEC_IR_decoding(u8* num);

#endif
