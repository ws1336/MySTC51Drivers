#include "Digital_to_String.h"
void Str_add(char* str,char* str_end)  //叠加字符串
{
  u8 i=0,j=0;
	while(str[i]!='\0')     
	{
		i++;
	}	
	while(str_end[j]!='\0')     
	{
		str[i] = str_end[j];
		i++;
		j++;
	}
	str[i]  = '\0';	
}
void Str_reversal(char* str)     //颠倒字符串
{
   u8 i=0,j=0;
	char temp;
	 while(str[j]!='\0')
		 j++;
	 if(j>0)
		j--;
	 while(j>i)
	 {
		 temp=str[i];
		 str[i]=str[j];
		 str[j]=temp;
		 i++;
		 j--;
	 }	  
}
void transform_int(unsigned long num,char* str) //数字转字符串（整数）
{
	u8 i,j;
	 if(num==0)
	{
		str[0]= '0';        	
		str[1]='\0';
	}
	else
	{	
		for(i=0;num>0;i++)    
		{                         
			j=num%10;           
			num=num/10;     
			str[i]= j+0x30;       
		}                         								
		str[i]= '\0';           
	}
}
void transform_dec(unsigned long num,char* str) //数字转字符串（小数）
{
	int i,j;
	 if(num==0)
	{
		str[0]= '0';        	
		str[1]='\0';
	}
	else
	{	
		for(i=8;i>=0;i--)    
		{                         
			j=num%10;           
			num=num/10;     
			str[i]= j+0x30;       
		}
		str[9]= '\0';           
	}
}
char* digital_to_string(double num,uchar range)
{
	static char inte[11],deci[10],resrlt[22];
	code const char err[]="ERROR!";	
	double dec;   //小数部分暂存
	char sign='+';  //+-符号
	u8 i;//临时变量
	 
	
	//最大 4294967295(由于浮点数精度问题大于4294967168都等于4294967295)
	unsigned long integer=0,decimal=0;
		
  /*********把浮点数的整数部分和小数部分分别保存到两个u32变量中************/
	
	if(num>=0)                       //保存整数部分
	{	                              //
		integer=(unsigned long)num;   //
		dec=(num-integer);            //
	}                               //
	else                            //
	{	                              //
		sign = '-';	                  //调整符号位
		integer=(unsigned long)-num;  //
		dec=-(num+integer);           //
	}	                              //

	for(i=0;i<9;i++)            		//保存小数部分
		  dec*=10;                    //
	decimal=(unsigned long)dec;     //

	if (integer>=4294967295)        //检查取值范围
		return err;                   //返回错误警告
	
	if(range>9)                     //检查精度是否符合要求
		range = 9;                    //不符合修改为最大精度
	

	/************************转化成字符串*****************************/
	
	inte[0]='\0';             //清空所有字符串
	deci[0]='\0';             //
	resrlt[0]='\0';           //
	
	transform_int(integer,inte);  //整数部分转换成字符串（颠倒的），至于为什么自己体会
	Str_reversal(inte);       		//反转整数部分字符串
  transform_dec(decimal,deci);  //小数部分转换成字符串	

	
	deci[range]='\0';         //调成小数精度
	
	if(sign=='-')             //连接字符串
		Str_add(resrlt,"-");    //符号位
	Str_add(resrlt,inte);     //整数部分
	if(range>0)               //如果精度为0，则舍去小数部分
	{
		Str_add(resrlt,".");    //小数点
		Str_add(resrlt,deci);   //小数部分
	}
	
	return resrlt;            //返回字符串指针
}