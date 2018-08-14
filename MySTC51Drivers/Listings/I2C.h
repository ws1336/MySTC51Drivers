#ifndef __I2C_h__
#define __I2C_h__
#include"define.h"
#include<intrins.h>
#include"DELAY.h"	
/**********模拟I2C通信驱动*************/

/******接口定义******/
sbit SCL = P1^1;
sbit SDA = P1^2;

/*（器件地址,数据地址,要写/读的数组的地址）*/
uchar I2C_write_onedata(uchar Device_address,uchar word_address,uchar dat);
uchar I2C_read_onedata(uchar Device_address,uchar word_address,uchar *dat);
/*（器件地址,数据地址,要写/读的数组的地址,读/写的数据个数）*/
uchar I2C_write_data(uchar Device_address,uchar word_address,uchar *dat,uint n);
uchar I2C_read_data(uchar Device_address,uchar word_address,uchar *dat,uint n);
#endif
