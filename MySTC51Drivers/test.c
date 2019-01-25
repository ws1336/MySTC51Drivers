#include "define.h"
#include "delay.h"
#include "Uart.h"


/*********************************************************************************
使用说明：

首先    如使用STC系列单片机，请在ISP烧录器软件的Keil仿真器设置中添加头文件到Keil中，
        若不使用，则编译报错，可修改define.h中包含的头文件

其次    请先在define.h中调整晶振频率与周期配置

*********************************************************************************/


void main()
{
    int time = 0;
    delay_ms(1000);
	uart_init(); //19200
	while(1)
	{   
        time ++;
        printf("Run Time : %ds\n",time);
        delay_ms(1000);
	}
}
