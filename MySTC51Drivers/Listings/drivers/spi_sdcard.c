#include "spi_sdcard.h"
#include "uart.h"

//预定义SD卡类型 
u8  SD_Type=0;

void spi_init()
{
    SD_CS = 1;
    SD_SCLK = 1;
    SD_MOSI = 1;
    SD_MISO = 1;
    delay_ms(1);
}
void spi_write(u8 sddat)
{
	u8 i;
	for(i=8; i; i--)
	{
		SD_SCLK = 0;
        delay5us();
		SD_MOSI = (sddat & 0x80);
		sddat <<= 1;
		SD_SCLK = 1;
        delay5us();
	}
	SD_MOSI = 1; 	
}

u8 spi_read(void)
{
	u8 Byte;
	u8 cnt;
	for(cnt=8; cnt; cnt--)
	{
		SD_SCLK = 0; 
        delay5us();        
		SD_SCLK = 1;
		Byte <<= 1;
		if(SD_MISO) 
            Byte++;
        delay5us(); 
	}
	return Byte;	
}

static u8 SD_WaitReady(void)  
{  
    u32 t=0;  
    do  
    {  
        if(spi_read()==0XFF)return 0;//OK  
        t++;        
    }while(t<0XFFFFFFFF);//等待
    return 1;  
}  

void SD_DisSelect(void)
{
    SD_CS=1;  
    spi_read(); 
}

u8 SD_Select(void)  
{  
    SD_CS=0;  
    if(SD_WaitReady()==0)return 0;//等待成功
    SD_DisSelect();  
    return 1;//等待失败
}  

u8 SD_SendCmd(u8 cmd, u32 arg, u8 crc)  
{  
    u8 r1;    
    u8 Retry=0;   
    SD_DisSelect();//取消上次片选
    if(cmd!=CMD0){   
        if(SD_Select())return 0XFF;//片选失败   加此语句为何不成功
    }
    SD_CS = 0;
    //发送
    spi_write(cmd | 0x40);//分别写入命令
    spi_write(arg >> 24);  
    spi_write(arg >> 16);  
    spi_write(arg >> 8);  
    spi_write(arg);        
    spi_write(crc);
    
    if(cmd==CMD12)spi_read();//Skip a stuff byte when stop reading  
    //等待响应或超时退出
    Retry=0X1F;  
    do  
    {  
        r1=spi_read();  
    }while((r1&0X80) && Retry--);      
    //返回状态值 
    return r1;      
}

u8 SD_GetResponse(u8 Response)  
{  
    u16 Count=0xFFF;//等待次数                              
    while ((spi_read()!=Response)&&Count)Count--;//等待得到准确的回应       
    if (Count==0)return MSD_RESPONSE_FAILURE;//得到失败的回应     
    else return MSD_RESPONSE_NO_ERROR;//正确回应
}  

u8 SD_Initialize(void)
{
    u16 i;
    u16 retry;
    u8 buf[4];
    u8 r1;
    
    spi_init();
    for(i=0;i<10;i++)spi_read(); //发送至少74个脉冲
    retry =20;
    do
    {
        r1 = SD_SendCmd(CMD0,0,0x95);
    }while((r1 != 0x01) && retry--);
    SD_Type=0;
    if(r1 == 0x01)
    {
        if(SD_SendCmd(CMD8,0x1AA,0x87)==1)//SD V2.0
        {  
            for(i=0;i<4;i++)buf[i]=spi_read(); //Get trailing return value of R7 resp
            if(buf[2]==0X01&&buf[3]==0XAA)//卡是否支持2.7~3.6V
            {  
                retry=0XFFFE;
                do  
                {  
                    SD_SendCmd(CMD55,0,0X01);    //发送CMD55
                    r1=SD_SendCmd(CMD41,0x40000000,0X01);//发送CMD41
                }while(r1&&retry--);  
                if(retry&&SD_SendCmd(CMD58,0,0X01)==0)//鉴别SD2.0版本开始
                {  
                    for(i=0;i<4;i++)buf[i]=spi_read();//得到OCR值
                    if(buf[0]&0x40)SD_Type=SD_TYPE_V2HC;    //检查CCS
                    else SD_Type=SD_TYPE_V2;     
                }  
            }  
        }else//SD V1.x/ MMC V3  
        {  
            SD_SendCmd(CMD55,0,0X01);        //发送CMD55  
            r1=SD_SendCmd(CMD41,0,0X01); //发送CMD41  
            if(r1<=1)  
            {         
                SD_Type=SD_TYPE_V1;  
                retry=0XFFFE;  
                do //等待退出IDLE模式
                {  
                    SD_SendCmd(CMD55,0,0X01);    //发送CMD55  
                    r1=SD_SendCmd(CMD41,0,0X01);//发送CMD41  
                }while(r1&&retry--);  
            }else  
            {  
                SD_Type=SD_TYPE_MMC;//MMC V3  
                retry=0XFFFE;  
                do //等待退出IDLE模式 
                {                                                 
                    r1=SD_SendCmd(CMD1,0,0X01);//发送CMD1  
                }while(r1&&retry--);    
            }  
            if(retry==0||SD_SendCmd(CMD16,512,0X01)!=0)SD_Type=SD_TYPE_ERR;//错误的卡
        }
    }
    SD_DisSelect();//取消片选
    if(SD_Type)return 0;  
    else if(r1)return r1;        
    return 0xaa;//其他错误
}

#if USE_PETIT_FATFS == 0
u8 SD_RecvData(u8*buf,u16 len)  
{                   
    if(SD_GetResponse(0xFE))return 1;//等待SD卡发挥数据起始令牌0xFE  
    while(len--)//开始接收数据  
    {  
        *buf=spi_read();  
        buf++;  
    }  
    //发送两个伪CRC(dummy CRC)  
    spi_read();  
    spi_read();                                                             
    return 0;//读取成功
}  

u8 SD_ReadDisk(u8*buf,u32 sector,u8 cnt)  
{  
    u8 r1;  
    if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;//转换为字节地址
    if(cnt==1)  
    {  
        r1=SD_SendCmd(CMD17,sector,0X01);//读命令 
        if(r1==0)//指令发送成功
        {  
            r1=SD_RecvData(buf,512);//接收512个字节   
        }  
    }else  
    {  
        r1=SD_SendCmd(CMD18,sector,0X01);//连续读命令
        do  
        {  
            r1=SD_RecvData(buf,512);//接收512字节      
            buf+=512;    
        }while(--cnt && r1==0);       
        SD_SendCmd(CMD12,0,0X01);    //发送停止命令
    }     
    SD_DisSelect();//取消片选
    return r1;//  
}
#if USE_WRITE_DISK

u8 SD_SendBlock(u8*buf,u8 cmd)  
{     
    u16 t;              
    if(SD_WaitReady())return 1;//等待准备失效
    spi_write(cmd);  
    if(cmd!=0XFD)//不是结束指令
    {  
        for(t=0;t<512;t++)spi_write(buf[t]);//
        spi_read();//忽略crc  
        spi_read();  
        t=spi_read();//接收响应  
        if((t&0x1F)!=0x05)return 2;//响应错误                                                     
    }                                                                                     
    return 0;//写入成功
} 

u8 SD_WriteDisk(u8*buf,u32 sector,u8 cnt)  
{  
    u8 r1;  
    if(SD_Type!=SD_TYPE_V2HC)sector *= 512;//转换为字节地址  
    if(cnt==1)  
    {  
        r1=SD_SendCmd(CMD24,sector,0X01);//写命令
        if(r1==0)
        {  
            r1=SD_SendBlock(buf,0xFE);//写512字节     
        }  
    }else  
    {  
        if(SD_Type!=SD_TYPE_MMC)  
        {  
            SD_SendCmd(CMD55,0,0X01);      
            SD_SendCmd(CMD23,cnt,0X01);//发送指令
        }  
        r1=SD_SendCmd(CMD25,sector,0X01);//连续写命令
        if(r1==0)  
        {  
            do  
            {  
                r1=SD_SendBlock(buf,0xFC);//写512字节    
                buf+=512;    
            }while(--cnt && r1==0);  
            r1=SD_SendBlock(0,0xFD);//写512字节结束  
        }  
    }     
    SD_DisSelect();//取消片选
    return r1;//  
} 
#endif
#else
u8 SD_RecvData(u8*buf,u16 offset, u16 count)  
{       
    u16 i=512-offset-count;
    if(SD_GetResponse(0xFE))return 1;//等待SD卡发挥数据起始令牌0xFE  
    while(offset--)//开始接收前置数据
    {  
        spi_read();  
    }
    while(count--)//开始接收数据  
    {  
        *buf=spi_read();  
        buf++;  
    }
    while(i--)//开始接收后补数据
    {  
        spi_read();  
    }    
    //发送两个伪CRC(dummy CRC)  
    spi_read();  
    spi_read();                                                             
    return 0;//读取成功
}  

u8 SD_ReadDisk(u8*buf,u32 sector,u16 offset, u16 count)  
{  
    u8 r1;  
    if(offset+count>512) return 3;
    if(SD_Type!=SD_TYPE_V2HC)sector <<= 9;//转换为字节地址
    
    r1=SD_SendCmd(CMD17,sector,0X01);//读命令 
    if(r1==0)//指令发送成功
    {  
        r1=SD_RecvData(buf,offset,count);//接收512个字节   
    }  
    
    SD_DisSelect();//取消片选
    return r1;//  
}  

#if USE_WRITE_DISK
u8 SD_SendBlock(u8*buf,u16 count)  
{     
    u16 t;              
    for(t=0;t<count;t++)spi_write(buf[t]);//                                                                               
    return 0;//写入成功
}  

u8 SD_WriteDisk(u8*buf,u32 sector)  
{  
    u8 r1=0; 
    u16 t;
    static u32 count=0;
    if (!buf) {
		if (sector) {
			// Initiate write process
            if(SD_Type!=SD_TYPE_V2HC)sector *= 512;//转换为字节地址  
            r1=SD_SendCmd(CMD24,sector,0X01);//写命令
            if(r1 == 0)
            {
                if(SD_WaitReady())return 1;//等待准备失效
                spi_write(0xFE);  //起始令牌
            }
            count=0;
		} else {
			// Finalize write process
            for(t=0;t<512-count;t++)spi_write(0);//  
            spi_read();//忽略crc  
            spi_read();  
            r1=spi_read();//接收响应  
            if((r1&0x1F)!=0x05) r1 = 2;//响应错误   
            else r1 = 0;
            SD_DisSelect();//取消片选
		}
	} else {
		// Send data to the disk
        count+=sector;
        if(count > 512) 
        {
            count-=sector;
            sector = 512 - count;
            count = 512;
        }
        r1=SD_SendBlock(buf,sector);//写sector个字节     
	}        
    return r1;//  
} 
#endif
#endif