#include "define.h"
#include "delay.h"
#include "Uart.h"

/********请先在define.h中调整晶振频率与周期配置**************/

void main()
{
    delay_ms(100);
	uart_init(); //19200
	while(1)
	{   
        printf("Hello World!");
        delay_ms(1000);
	}
}
