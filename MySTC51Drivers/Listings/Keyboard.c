#include "Keyboard.h"
/*********键盘默认接P3口*********/
uchar keyscan()
{

  uchar l,h;
	INDEPEND_KEY_PORT = 0xff;
	if(INDEPEND_KEY_PORT!=0xff)
		return INDEPEND_KEY_PORT;
	KEY_BOARD_PORT = 0xf0;
	if(KEY_BOARD_PORT==0xf0)
		return 0;                //检测不到有按键按下 返回0
	if(KEY_BOARD_PORT != 0xf0)
	{
		l = KEY_BOARD_PORT;
		KEY_BOARD_PORT = 0x0f;
		if(KEY_BOARD_PORT==0x0f)
			return 0;              //检测不到有按键按下 返回0
		h = KEY_BOARD_PORT;
	}
	return (h+l);
}
uchar Key_read()
{
	uchar value;
	switch(keyscan())
	{
		case 0xee : value = 1;break;
		case 0xde : value = 2;break;
		case 0xbe : value = 3;break;
		case 0x7e : value = 4;break;

		case 0xed : value = 5;break;
		case 0xdd : value = 6;break;
		case 0xbd : value = 7;break;
		case 0x7d : value = 8;break;

		case 0xeb : value = 9;break;
		case 0xdb : value = 10;break;
		case 0xbb : value = 11;break;
		case 0x7b : value = 12;break;

		case 0xe7 : value = 13;break;
		case 0xd7 : value = 14;break;
		case 0xb7 : value = 15;break;
		case 0x77 : value = 16;break;
		
		case 0xfe : value = 17;break;
		case 0xfd : value = 18;break;
		case 0xfb : value = 19;break;
		case 0xf7 : value = 20;break;
		default : value = 0;            //检测不到正确的按键 返回0
	}
	return value;                     //按键返回值
}
